#include "patch.h"
#include "sqlite3.h"

#include <iostream>

using namespace std;

void trace_callback( void* udp, const char* sql ) { printf("{SQL} [%s]\n", sql); }

const char* usage = "Usage: sqlite-patch [db] [patchfile]";

int main(int argc, char const *argv[])
{
	if (argc != 3) {
        cerr << "Wrong number of arguments" << endl << usage << endl;
		return 1;
	}

	const char* dbFile = argv[1];
	const char* patchFile = argv[2];

	int rc;
	sqlite3* db;
	rc = sqlite3_open_v2(dbFile, &db, SQLITE_OPEN_READWRITE, nullptr);

	if (rc != SQLITE_OK) {
		cerr << "Could not open sqlite DB " << dbFile << ": " << sqlite3_errstr(rc) << endl;
		sqlite3_close(db);
		return 2;
	}

	rc = applyChangeset(db, patchFile);

	if (rc != SQLITE_OK) {
		cerr << "Could not apply changeset " << patchFile << endl;
		sqlite3_close(db);
		return 2;	
	}

	sqlite3_close(db);

	return 0;
}
