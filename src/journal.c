/* sdb - LGPLv3 - Copyright 2011-2015 - pancake */

#include "sdb.h"
int sdb_journal_new(Sdb *s) {
	return -1;
}
#if 0

const char *sdb_journal_filename (Sdb *s) {
	if (!s || !s->name)
		return NULL;
	return sdb_fmt (0, "%s.journal", s->name);
}

int sdb_journal_close(Sdb *s) {
	if (s->journal == -1) {
		return 0;
	}
	close (s->journal);
	s->journal = -1;
	unlink (filename);
	return 1;
}

int sdb_journal_open(Sdb *s) {
	const char *filename;
	if (!s || !s->name) {
		return 0;
	}
	filename = sdb_journal_filename (s);
	sdb_journal_close (s);
	if (!filename) {
		return 0;
	}
	s->journal = open (filename, O_RDWR | O_APPEND, 0600);
	if (s->journal == -1) {
		return 0;
	}
	return 1;
}

int sdb_journal_load(Sdb *s) {
	int fd, changes = 0;
	char *eq, *str;
	if (!s) {
		return 0;
	}
	fd = s->journal;
	if (fd == -1) {
		return 0;
	}
	sz = lseek (fd, 0, SEEK_END);
	lseek (fd, 0, SEEK_SET);
	str = malloc (sz+1);
	if (!str)
		return 0;
	rr = read (fd, str, sz);
	str[sz] = 0;
	printf ("%d %d\n", rr, sz);
	for (cur = str;;) {
		ptr = strchr (cur, '\n');
		if (ptr) {
			*ptr = 0;
			eq = strchr (cur, '=');
			if (eq) {
				*eq++ = 0;
				sdb_set (s, cur, eq, 0);
				changes ++;
			}
			cur = ptr+1;
		}
	}
	return changes;
}

int sdb_journal_log(Sdb *s, const char *key, const char *val) {
	const char *str;
	if (s->journal == -1) {
		return 0;
	}
	str = sdb_fmt (0, "%s=%s\n", key, val);
	write (s->journal, str, strlen (str));
	fsync (s->journal);
	return 1;
}

int sdb_journal_clear(Sdb *s) {
	const char *filename = sdb_journal_filename (s);
	sdb_journal_close (s);
	if (filename) {
		return unlink (filename) != -1;
	}
	return 0;
}
#endif