#include "diff.h"
#include "sqlite3.h"

#include <iostream>
#include <string>

using namespace std;

void trace_callback(void* udp, const char* sql) {
	std::cerr << "{SQL} " << sql << std::endl;
}

#define SQLOK(X) do {														\
	rc = X; 																\
	if (rc != 0) {															\
		std::cerr << "Error: " << #X << " returned " << rc << std::endl;	\
		std::cerr << sqlite3_errstr(rc) << std::endl;						\
		return rc;															\
	}																		\
} while(0)

const char* usage = "Usage: sqlite-diff [db1] [db2]";

int main(int argc, char const *argv[])
{
	if (argc != 3) {
        cerr << "Wrong number of arguments" << endl << usage << endl;
		return 1;
	}

	const char* db1File = argv[1];
	const char* db2File = argv[2];

	int rc;
	sqlite3* db;
	rc = sqlite3_open_v2(db1File, &db, SQLITE_OPEN_READWRITE, nullptr);

	//sqlite3_trace(db, trace_callback, NULL);

	if (rc != SQLITE_OK) {
		cerr << "Could not open sqlite DB " << db1File << endl;
		return 2;
	}

	SQLOK(sqlite3_exec(db, (string() + "ATTACH '" + db2File + "' AS 'aux';").data(), 0, 0, 0));

	sqlitediff_diff_prepared(
		db,
		nullptr,
		stdout
	);

	if (rc != SQLITE_OK) {
		cerr << "Could not create changeset." << endl;
		sqlite3_close(db);
		return 2;	
	}

	sqlite3_close(db);

	return 0;
}
