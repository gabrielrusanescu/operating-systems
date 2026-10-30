#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * How many records we add to the database every time it runs out of room.
 * The array grows by whole chunks, never one record at a time, and it is
 * never shrunk.
 */
#define CHUNK		4

#define NAME_LEN	32

/* One row in our database. */
struct record {
	int id;
	char name[NAME_LEN];
};

/*
 * The database itself.
 *
 *   records  -- pointer to a heap array of `capacity` records
 *   count    -- how many records are actually in use
 *   capacity -- how many records the array can hold before it must grow
 *
 * Invariant: count <= capacity.
 */
struct database {
	struct record *records;
	size_t count;
	size_t capacity;
};

/*
 * TODO 1: start with an empty database.
 *
 * An empty database owns no memory.  Set the three fields to say so, in a
 * way that lets the first db_add() find out that it has to allocate.
 */
void db_init(struct database *db)
{
	/* TODO 1 */
	db -> records = NULL;
	db -> count = 0;
	db -> capacity = 0;
}

/*
 * TODO 2: add one record to the database, growing the array by CHUNK
 * records when there is no room left.
 *
 * db->count is where the new record goes and, once it is there, how many
 * records there are.  Keep count <= capacity true at every step.
 *
 * Before writing the grow step, read what realloc() does when it fails,
 * and what that means for the pointer you passed it.
 *
 * Return 0 on success, -1 if memory ran out.
 */
int db_add(struct database *db, int id, const char *name)
{
	/* TODO 2 */
	if (db -> count == db -> capacity) {
		db -> capacity += CHUNK;
		void *ptr = realloc(db -> records, db -> capacity * sizeof(struct record));
		if (ptr == NULL)
			return -1;
		db->records = ptr;
	}
	db->records[db->count].id = id;
	strcpy(db->records[db->count].name, name);
	db->count++;
	return 0;
}

/* Print every record, one per line. Already written for you. */
void db_print(const struct database *db)
{
	printf("Database holds %zu record(s) (capacity %zu):\n",
	       db->count, db->capacity);

	for (size_t i = 0; i < db->count; i++)
		printf("  [%d] %s\n", db->records[i].id, db->records[i].name);
}

int db_remove(struct database *db, int id)
{
	size_t i = 0;
	while (i < db->count) {
		if (db->records[i].id == id) {
				for (size_t j = i; j < db->count - 1; ++j)
					db->records[j] = db->records[j+1];
				//break;
				db->count--;
				if (db->capacity >= CHUNK && db->count <= db->capacity - CHUNK) {
					if (db->capacity - CHUNK == 0) {
						free(db->records);
						db->records = NULL;
					}
					else {
						void *ptr = realloc(db->records, (db->capacity - CHUNK) * sizeof(struct record));
						if (ptr == NULL) {
							perror("realloc");
							return -1;
						}
						db->records = ptr;
					}
				db->capacity = db->capacity - CHUNK;
				}
				return 0;
			}
		++i;
	}
	return -1;
}


/*
 * TODO 3: release everything the database owns, and leave it in a state
 * in which another db_add() or db_free() would be safe.
 */
void db_free(struct database *db)
{
	/* TODO 3 */
	free(db->records);
	db->records = NULL;
	db->count=0;
	db->capacity=0;
}

/*
 * Read records from stdin until end-of-file. Each line is:
 *
 *     <id> <name>
 *
 * for example:
 *
 *     1 alice
 *     2 bob
 *
 * Finish input with Ctrl-D (or by piping a file in).
 */
int main(void)
{
	struct database db;
	int id;
	char name[NAME_LEN];

	db_init(&db);
	/*
	while (scanf("%d %31s", &id, name) == 2) {
		if (db_add(&db, id, name) != 0) {
			fprintf(stderr, "out of memory\n");
			goto fail;
		}
	}*/
	char cmd[10];
	while (scanf("%9s", cmd) == 1) {
     	if (strcmp(cmd, "add") == 0) {
       	 	 scanf("%d %31s", &id, name);
          	if (db_add(&db, id, name) != 0) {
              	fprintf(stderr, "0 memory\n");
              	goto fail;
          	}
      	} else if (strcmp(cmd, "del") == 0) {
          	scanf("%d", &id);
          	db_remove(&db, id);
      	}
	}


	db_print(&db);
	db_free(&db);

	return 0;

fail:
	db_free(&db);

	return -1;
}
