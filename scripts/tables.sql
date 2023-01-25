use Aste_Online;

CREATE TABLE Categorie (
	`1° Livello` VARCHAR(32) NOT NULL,
	`2° Livello` VARCHAR(32) NOT NULL,
	`3° Livello` VARCHAR(32) NOT NULL,
	PRIMARY KEY(`1° Livello`, `2° Livello`, `3° Livello`)
);

CREATE TABLE Utenti (
	CF CHAR(16) NOT NULL PRIMARY KEY,
	Nome VARCHAR(32) NOT NULL,
	Cognome VARCHAR(32) NOT NULL,
	Indirizzo VARCHAR(64) NOT NULL,
	`Data di nascita` DATE NOT NULL,
	`Città di nascita` VARCHAR(32) NOT NULL
);

CREATE TABLE Oggetti (
	Codice CHAR(7) NOT NULL PRIMARY KEY,
	Stato VARCHAR(32) NOT NULL,
	Dimensioni VARCHAR(32) NOT NULL,
	Descrizione BLOB NOT NULL,
	`Scadenza Asta` DATETIME NOT NULL,
	`Numero di offerte` SMALLINT UNSIGNED NOT NULL,
	`1° Livello` VARCHAR(32) NOT NULL,
	`2° Livello` VARCHAR(32) NOT NULL,
	`3° Livello` VARCHAR(32) NOT NULL
);

CREATE TABLE `Carte di credito` (
	Numero CHAR(19) NOT NULL,
	Intestatario CHAR(16) NOT NULL,
	CVV SMALLINT UNSIGNED NOT NULL,
	`Data di scadenza` VARCHAR(64) NOT NULL,
	FOREIGN KEY (Intestatario) REFERENCES Utenti(CF),
    PRIMARY KEY (Intestatario, Numero)
);

CREATE TABLE Controfferte (
	Importo FLOAT NOT NULL,
	Utente CHAR(16) NOT NULL,
	Oggetto CHAR(7) NOT NULL,
	FOREIGN KEY (Utente) REFERENCES Utenti(CF),
	FOREIGN KEY (Oggetto) REFERENCES Oggetti(Codice),
	PRIMARY KEY (Importo, Utente, Oggetto)
);

CREATE TABLE Offerte (
	`Timestamp` DATETIME NOT NULL,
	Utente CHAR(16) NOT NULL,
	Importo FLOAT NOT NULL,
	Oggetto CHAR(7) NOT NULL,
	FOREIGN KEY (Utente) REFERENCES Utenti(CF),
	FOREIGN KEY (Oggetto) REFERENCES Oggetti(Codice),
	PRIMARY KEY (`Timestamp`, Utente)
);
