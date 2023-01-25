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
        print(f"\t{c[0]} {c[1]}{check_attribute(c[2], ' NOT NULL')}{check_attribute(c[3], ' PRIMARY KEY')}{' FOREIGN KEY REFERENCES ' + c[4][0] + '(' + c[4][1] + ')' if c[4] else ''}{',' if c != args[-1] else ''}")
    
    print(");\n")

def create_insert_query(table_name: str, *args: value):
    print(f"INSERT INTO {table_name} ({', '.join(v[0] for v in args)})")
    print(f"VALUES ({', '.join(v[1] for v in args)})")


column1 = ("Codice", "CHAR(7)", 1, 1, ())
column2 = ("Stato", "VARCHAR(32)", 1, 0, ())
column3 = ("Dimensioni", "VARCHAR(32)", 1, 0, ())
column4 = ("Descrizione", "VARCHAR(65535)", 1, 0, ())
column5 = ("Scadenza Asta", "DATETIME", 1, 0, ())
column6 = ("Numero di offerte", "SMALLINT UNSIGNED", 1, 0, ())
column7 = ("1° Livello", "VARCHAR(32)", 1, 0, ("Categorie", "1° livello"))
column8 = ("2° Livello", "VARCHAR(32)", 1, 0, ("Categorie", "2° livello"))
column9 = ("3° Livello", "VARCHAR(32)", 1, 0, ("Categorie", "3° livello"))

create_table_query("Oggetti", column1, column2, column3, column4, column5, column6, column7, column8, column9)

column1 = ("1° Livello", "VARCHAR(32)", 1, 1, ())
column2 = ("2° Livello", "VARCHAR(32)", 1, 1, ())
column3 = ("3° Livello", "VARCHAR(32)", 1, 1, ())

create_table_query("Categorie", column1, column2, column3)

column1 = ("CF", "CHAR(16)", 1, 1, ())
column2 = ("Nome", "VARCHAR(32)", 1, 0, ())
column3 = ("Cognome", "VARCHAR(32)", 1, 0, ())
column4 = ("Indirizzo", "VARCHAR(64)", 1, 0, ())
column5 = ("Data di nascita", "DATE", 1, 0, ())
column6 = ("Città di nascita", "VARCHAR(32)", 1, 0, ())

create_table_query("Utenti", column1, column2, column3, column4, column5, column6)

column1 = ("Numero", "CHAR(19)", 1, 1, ())
column2 = ("Intestatario", "CHAR(16)", 1, 1, ("Utenti", "CF"))
column3 = ("CVV", "UNSIGNED SMALLINT(2)", 1, 0, ())
column4 = ("Data di scadenza", "VARCHAR(64)", 1, 0, ())

create_table_query("Carte di credito", column1, column2, column3, column4)

column1 = ("Importo", "FLOAT", 1, 1, ())
column2 = ("Utente", "CHAR(16)", 1, 1, ("Utenti", "CF"))
column3 = ("Oggetto", "CHAR(7)", 1, 0, ("Oggetto", "CF"))

create_table_query("Controfferte", column1, column2, column3)

column1 = ("Timestamp", "DATETIME", 1, 1, ())
column2 = ("Utente", "CHAR(16)", 1, 1, ("Utenti", "CF"))
column3 = ("Importo", "FLOAT", 1, 0, ())
column4 = ("Oggetto", "CHAR(7)", 1, 0, ("Oggetto", "CF"))

create_table_query("Offerte", column1, column2, column3, column4)
