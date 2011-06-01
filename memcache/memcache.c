#include "memcache.h"

MemcacheSdb *memcache_sdb_new (const char *file) {
	MemcacheSdb *ms;
	sdb *s = sdb_new (file, R_FALSE);
	if (!s) return NULL;
	ms = R_NEW (MemcacheSdb);
	if (!ms) {
		sdb_free (s);
		return NULL;
	}
	ms->sdb = s;
	return ms;
}

void memcache_free (MemcacheSdb *ms) {
	sdb_sync (ms->sdb);
	sdb_free (ms->sdb);
	free (ms);
}

/* storage */
char *memcache_incr(MemcacheSdb *ms, const char *key, ut64 val) {
	if (sdb_inc (ms->sdb, key, val) == UT64_MAX)
		return NULL;
	return sdb_get (ms->sdb, key);
}

char *memcache_decr(MemcacheSdb *ms, const char *key, ut64 val) {
	return memcache_incr (ms, key, -val);
}

void memcache_set(MemcacheSdb *ms, const char *key, ut64 exptime, const ut8 *body) {
	sdb_set (ms->sdb, key, body);
}

int memcache_add(MemcacheSdb *ms, const char *key, ut64 exptime, const ut8 *body) {
	if (!sdb_exists (ms->sdb, key)) {
		sdb_set (ms->sdb, key, body);
		return 1;
	}
	return 0;
}

void memcache_append(MemcacheSdb *ms, const char *key, const ut8 *body) {
	int len = strlen (body);
	char *a, *b;
	a = sdb_get (ms->sdb, key);
	if (a) {
		b = malloc (1 + len + a?strlen (a):0);
		strcpy (b, body);
		strcpy (b+len, a);
		sdb_set (ms->sdb, key, b);
		free (b);
		free (a);
	} else sdb_set (ms->sdb, key, body);
}

void memcache_prepend(MemcacheSdb *ms, const char *key, const ut8 *body) {
	int len = strlen (body);
	char *a, *b;
	a = sdb_get (ms->sdb, key);
	if (a) {
		b = malloc (1 + len + strlen (a));
		strcpy (b, a);
		strcpy (b+len, body);
		sdb_set (ms->sdb, key, b);
		free (b);
		free (a);
	} else sdb_set (ms->sdb, key, body);
}

int memcache_replace(MemcacheSdb *ms, const char *key, ut64 exptime, const ut8 *body) {
	if (sdb_exists (ms->sdb, key)) {
		sdb_set (ms->sdb, key, body);
		return 1;
	}
	return 0;
}

/*
  NOT IMPLEMENTED
  "cas" is a check and set operation which means "store this data but
  only if no one else has updated since I last fetched it."
*/
void memcache_cas(MemcacheSdb *ms, const char *key, ut64 exptime, const char *body) {
#warning memcache_cas not implemented
	memcache_set (ms, key, exptime, body);
}

/* retrieval */
char *memcache_get (MemcacheSdb *ms, const char *key, ut64 *exptime) {
	char *s = sdb_get (ms->sdb, key);
	*exptime = 0;
	return s;
}
void memcache_gets ();

int memcache_delete(MemcacheSdb *ms, const char *key, ut64 exptime) {
	//TODO if (exptime)
	return sdb_delete (ms->sdb, key);
}

/* other */
void memcache_stats();
void memcache_version();
void memcache_quit ();