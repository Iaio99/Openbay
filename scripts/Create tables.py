from typing import NewType

check_attribute = lambda attribute, s : s if attribute else ''

ColumnName = NewType("ColumnName", str)
MySQLType = NewType("MySQLType", str)
NotNull = NewType("NotNull", bool)
PrimaryKey = NewType("PrimaryKey", bool)
ForeignKey = NewType("ForeignKey", tuple)

value = (str, str)
column = ColumnName, MySQLType, NotNull, PrimaryKey, ForeignKey

def create_table_query(table_name: str, *args: column):
    print(f"CREATE TABLE {table_name} (")
    for c in args:
        print(f"{c[0]} {c[1]}{check_attribute(c[2], ' NOT NULL')}{check_attribute(c[3], ' PRIMARY KEY')}{' FOREIGN KEY REFERENCES ' + c[4][0] + '(' + c[4][1] + ')' if c[4] else ''}{',' if c != args[-1] else ''}")
    
    print(");")

def create_insert_query(table_name: str, *args: value):
    print(f"INSERT INTO {table_name} ({', '.join(v[0] for v in args)})")
    print(f"VALUES ({', '.join(v[1] for v in args)})")


column1 = ("Nome", "VARCHAR(16)", 1, 1, ())
column2 = ("Cognome", "VARCHAR(16)", 1, 0, ())

v1 = ("Nome", "Giuliano")
v2 = ("Cognome", "Vallone")

create_table_query("Persone", column1, column2)
create_insert_query("Persone", v1, v2)