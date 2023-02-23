-- MariaDB dump 10.19  Distrib 10.11.2-MariaDB, for Linux (x86_64)
--
-- Host: localhost    Database: Openbay
-- ------------------------------------------------------
-- Server version	8.0.31

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Temporary table structure for view `Aste`
--

DROP TABLE IF EXISTS `Aste`;
/*!50001 DROP VIEW IF EXISTS `Aste`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `Aste` AS SELECT
 1 AS `Codice`,
  1 AS `Stato`,
  1 AS `Lunghezza`,
  1 AS `Larghezza`,
  1 AS `Altezza`,
  1 AS `Descrizione`,
  1 AS `BaseAsta`,
  1 AS `ScadenzaAsta`,
  1 AS `NumeroOfferte`,
  1 AS `PrimoLivello`,
  1 AS `SecondoLivello`,
  1 AS `TerzoLivello`,
  1 AS `OffertaMassima`,
  1 AS `Partecipante` */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `Auth`
--

DROP TABLE IF EXISTS `Auth`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Auth` (
  `Username` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Password` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Ruolo` enum('amministratore','utente') CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`Username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Auth`
--

LOCK TABLES `Auth` WRITE;
/*!40000 ALTER TABLE `Auth` DISABLE KEYS */;
/*!40000 ALTER TABLE `Auth` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `CarteCredito`
--

DROP TABLE IF EXISTS `CarteCredito`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `CarteCredito` (
  `Numero` char(19) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Intestatario` char(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `CVV` smallint unsigned NOT NULL,
  `DataScadenza` date NOT NULL,
  PRIMARY KEY (`Intestatario`,`Numero`),
  CONSTRAINT `CarteCredito_ibfk_1` FOREIGN KEY (`Intestatario`) REFERENCES `Utenti` (`CF`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `CarteCredito`
--

LOCK TABLES `CarteCredito` WRITE;
/*!40000 ALTER TABLE `CarteCredito` DISABLE KEYS */;
INSERT INTO `CarteCredito` VALUES
('523','2026-04-01',0,'0000-00-00');
/*!40000 ALTER TABLE `CarteCredito` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`giuliano`@`%`*/ /*!50003 TRIGGER `assert_card` BEFORE INSERT ON `CarteCredito` FOR EACH ROW BEGIN
	IF NOT New.Numero REGEXP "^([0-9]{4}-[0-9]{4}-[0-9]{4}-[0-9]{4})"  THEN
    	SIGNAL SQLSTATE "45001" SET MESSAGE_TEXT = "[InputError] The credit must be kind of XXXX-XXXX-XXXX-XXXX where X is a number!";
    END IF;
    
    IF New.CVV > 10000 OR NEW.CVV < 100 THEN 
    	SIGNAL SQLSTATE "45001" SET MESSAGE_TEXT = 
        "[InputError] The CVV must a number of digits between 3 and 4!";
    END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `Categorie`
--

DROP TABLE IF EXISTS `Categorie`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Categorie` (
  `PrimoLivello` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `SecondoLivello` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `TerzoLivello` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`PrimoLivello`,`SecondoLivello`,`TerzoLivello`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Categorie`
--

LOCK TABLES `Categorie` WRITE;
/*!40000 ALTER TABLE `Categorie` DISABLE KEYS */;
INSERT INTO `Categorie` VALUES
('Elettronica','Informatica','Videogiochi');
/*!40000 ALTER TABLE `Categorie` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`giuliano`@`%`*/ /*!50003 TRIGGER `assert_level_names` BEFORE INSERT ON `Categorie` FOR EACH ROW BEGIN
	IF NEW.`PrimoLivello` = NEW.`SecondoLivello` OR NEW.`PrimoLivello` = NEW.`TerzoLivello` OR NEW.`SecondoLivello` = NEW.`TerzoLivello` THEN
		SIGNAL SQLSTATE '45001' SET MESSAGE_TEXT = "[InputError] The levels's names must be all different!";
	END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `Controfferte`
--

DROP TABLE IF EXISTS `Controfferte`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Controfferte` (
  `Importo` float NOT NULL,
  `Oggetto` char(7) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Utente` char(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`Importo`,`Oggetto`),
  KEY `Utente` (`Utente`),
  KEY `Oggetto` (`Oggetto`),
  CONSTRAINT `Controfferte_ibfk_1` FOREIGN KEY (`Utente`) REFERENCES `Utenti` (`CF`),
  CONSTRAINT `Controfferte_ibfk_2` FOREIGN KEY (`Oggetto`) REFERENCES `Oggetti` (`Codice`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Controfferte`
--

LOCK TABLES `Controfferte` WRITE;
/*!40000 ALTER TABLE `Controfferte` DISABLE KEYS */;
/*!40000 ALTER TABLE `Controfferte` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`giuliano`@`%`*/ /*!50003 TRIGGER `assert_counteroffer` BEFORE INSERT ON `Controfferte` FOR EACH ROW BEGIN
	DECLARE increment FLOAT UNSIGNED;
    DECLARE importo_controfferta FLOAT UNSIGNED;
    DECLARE max_offerer CHAR(16);
    
    SET max_offerer = (SELECT Utente FROM Offerte Where Oggetto = New.Oggetto GROUP BY Oggetto HAVING MAX_OFFER(Oggetto));
    
    IF NEW.Utente != max_offerer THEN
    	SIGNAL SQLSTATE "45002" SET MESSAGE_TEXT = "[OfferError] You are not the max offerer!";
    END IF;
    
    SET importo_controfferta = (SELECT Importo FROM Controfferte WHERE Oggetto = New.Oggetto);
    
    IF Importo_controfferta <= MAX_OFFER(New.Oggetto) THEN
    	SIGNAL SQLSTATE "45001" SET MESSAGE_TEXT = "[InputError] The counteroffer must be higher then max offer";
    END IF;
    
    SET increment = CAST(((NEW.Importo-offerta_max)) AS DECIMAL)%0.5;
    IF increment != 0.0 THEN
    	SIGNAL SQLSTATE "45001" SET MESSAGE_TEXT = "[InputError] The increment must a multiple of 0.50€!";
    END IF;
    
	DELETE FROM Controfferte WHERE Oggetto=NEW.Oggetto;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `Offerte`
--

DROP TABLE IF EXISTS `Offerte`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Offerte` (
  `Importo` float unsigned NOT NULL,
  `Oggetto` char(7) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Utente` char(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Timestamp` timestamp NOT NULL,
  PRIMARY KEY (`Importo`,`Oggetto`),
  KEY `Utente` (`Utente`),
  KEY `Oggetto` (`Oggetto`),
  CONSTRAINT `Offerte_ibfk_1` FOREIGN KEY (`Utente`) REFERENCES `Utenti` (`CF`),
  CONSTRAINT `Offerte_ibfk_2` FOREIGN KEY (`Oggetto`) REFERENCES `Oggetti` (`Codice`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Offerte`
--

LOCK TABLES `Offerte` WRITE;
/*!40000 ALTER TABLE `Offerte` DISABLE KEYS */;
INSERT INTO `Offerte` VALUES
(20.3,'ee4881e','2023-02-15 15:08','0000-00-00 00:00:00');
/*!40000 ALTER TABLE `Offerte` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`giuliano`@`%`*/ /*!50003 TRIGGER `assert_offer` BEFORE INSERT ON `Offerte` FOR EACH ROW BEGIN
	DECLARE offerta_max FLOAT UNSIGNED;
	DECLARE increment FLOAT UNSIGNED;
    DECLARE max_offerer CHAR(16);
    
    SET offerta_max = MAX_OFFER(New.Oggetto);
    SET increment = (CAST(((NEW.Importo-offerta_max)) AS DECIMAL)%0.5);

    IF new.Importo < offerta_max THEN
		SIGNAL SQLSTATE '45002' SET MESSAGE_TEXT = "[InputError] Your offer is too low!";
    END IF;

	IF increment != 0.0 THEN
		SIGNAL SQLSTATE '45001' SET MESSAGE_TEXT = "[InputError] The increment must be a multiple of 0.50€";
	END IF;
    
    SET max_offerer = (SELECT Utente FROM Offerte Where Oggetto = New.Oggetto GROUP BY Oggetto HAVING MAX_OFFER(Oggetto));
    
    IF NEW.Utente = max_offerer THEN
    	SIGNAL SQLSTATE "45002" SET MESSAGE_TEXT = "[OfferError] You are already the max offerer!";
    END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`giuliano`@`%`*/ /*!50003 TRIGGER `increment_offers_number` AFTER INSERT ON `Offerte` FOR EACH ROW BEGIN
	SET @old_numero_offerte := (SELECT NumeroOfferte FROM Oggetti WHERE Codice=New.Oggetto);
    UPDATE Oggetti SET NumeroOfferte = @old_numero_offerte+1 WHERE Codice=New.Oggetto;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `Oggetti`
--

DROP TABLE IF EXISTS `Oggetti`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Oggetti` (
  `Codice` char(7) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Stato` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Larghezza` tinyint unsigned NOT NULL,
  `Altezza` tinyint unsigned NOT NULL,
  `Lunghezza` tinyint unsigned NOT NULL,
  `Descrizione` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `BaseAsta` float unsigned NOT NULL,
  `ScadenzaAsta` datetime NOT NULL,
  `NumeroOfferte` smallint unsigned NOT NULL,
  `PrimoLivello` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `SecondoLivello` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `TerzoLivello` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`Codice`),
  KEY `Oggetti_ibfk_1` (`PrimoLivello`,`SecondoLivello`,`TerzoLivello`),
  KEY `ScadenzaAsta` (`ScadenzaAsta`),
  CONSTRAINT `Oggetti_ibfk_1` FOREIGN KEY (`PrimoLivello`, `SecondoLivello`, `TerzoLivello`) REFERENCES `Categorie` (`PrimoLivello`, `SecondoLivello`, `TerzoLivello`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Oggetti`
--

LOCK TABLES `Oggetti` WRITE;
/*!40000 ALTER TABLE `Oggetti` DISABLE KEYS */;
/*!40000 ALTER TABLE `Oggetti` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Utenti`
--

DROP TABLE IF EXISTS `Utenti`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Utenti` (
  `CF` char(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Nome` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Cognome` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Indirizzo` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `DataNascita` date NOT NULL,
  `CittaNascita` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Username` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`CF`),
  KEY `Username` (`Username`),
  CONSTRAINT `Utenti_ibfk_1` FOREIGN KEY (`Username`) REFERENCES `Auth` (`Username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Utenti`
--

LOCK TABLES `Utenti` WRITE;
/*!40000 ALTER TABLE `Utenti` DISABLE KEYS */;
INSERT INTO `Utenti` VALUES
('VLLGLN99T02H501N','Rome','Vallone','1999-12-02','0000-00-00','Giuliano','Iaio99');
/*!40000 ALTER TABLE `Utenti` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`giuliano`@`%`*/ /*!50003 TRIGGER `assert_cf` BEFORE INSERT ON `Utenti` FOR EACH ROW BEGIN
	IF new.cf REGEXP '^(([A-Z]|[a-z]){6}[0-9]{2}([A-Z]|[a-z])[0-9]{2}([A-Z]|[a-z])[0-9]{3}([A-Z]|[a-z]))' THEN
    	SIGNAL SQLSTATE "45001" set MESSAGE_TEXT = "[InputError] The CF inserted is not valid!";
    END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Dumping routines for database 'Openbay'
--
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
/*!50003 DROP FUNCTION IF EXISTS `MAX_OFFER` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
DELIMITER ;;
CREATE DEFINER=`giuliano`@`%` FUNCTION `MAX_OFFER`(`codice` CHAR(7)) RETURNS float unsigned
    DETERMINISTIC
BEGIN
	DECLARE numero_offerte SMALLINT UNSIGNED;
    DECLARE base_asta FLOAT UNSIGNED;
    DECLARE offerta_massima FLOAT UNSIGNED;
    
	SELECT NumeroOfferte, BaseAsta, MAX(Offerte.Importo)
    FROM Oggetti LEFT JOIN Offerte ON Oggetti.Codice = Offerte.Oggetto
    WHERE Oggetti.Codice = codice INTO numero_offerte, base_asta, offerta_massima;

	IF offerta_massima IS NULL THEN
		return base_asta;
	ELSE
		return offerta_massima;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
/*!50003 DROP PROCEDURE IF EXISTS `auctions_in_progress_informations` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
DELIMITER ;;
CREATE DEFINER=`giuliano`@`%` PROCEDURE `auctions_in_progress_informations`()
BEGIN
	SELECT
`Codice`,
`Stato`,
`Lunghezza`,
`Larghezza`,
`Altezza`,
`Descrizione`,
`BaseAsta`,
`ScadenzaAsta`,
`PrimoLivello`,
`SecondoLivello`,
`TerzoLivello`,
`OffertaMassima`,
 `NumeroOfferte`
    FROM Aste
	WHERE `ScadenzaAsta` > CURRENT_TIMESTAMP()
    ORDER BY(ScadenzaAsta);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
/*!50003 DROP PROCEDURE IF EXISTS `call_auction` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
DELIMITER ;;
CREATE DEFINER=`giuliano`@`%` PROCEDURE `call_auction`(IN `stato` VARCHAR(32), IN `lunghezza` TINYINT UNSIGNED, IN `larghezza` TINYINT UNSIGNED, IN `altezza` TINYINT UNSIGNED, IN `descrizione` TEXT, IN `base_dasta` FLOAT UNSIGNED, IN `durata_asta` TINYINT, IN `primo_livello` VARCHAR(32), IN `secondo_livello` VARCHAR(32), IN `terzo_livello` VARCHAR(32))
BEGIN
	DECLARE EXIT HANDLER FOR SQLEXCEPTION, SQLSTATE "45001"
    BEGIN
    	ROLLBACK;
        RESIGNAL;
    END;

	IF durata_asta > 7 OR durata_asta < 1 THEN
    	SIGNAL SQLSTATE "45001" SET MESSAGE_TEXT = "[InputError] The duration of the asta must be between 1 and 7 days";
    END IF;

	INSERT INTO Oggetti VALUES (LEFT (UUID(),7), stato, lunghezza, larghezza, altezza, descrizione, base_dasta, DATE_ADD(NOW(), INTERVAL durata_asta DAY), 0, primo_livello, secondo_livello, terzo_livello);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
/*!50003 DROP PROCEDURE IF EXISTS `closed_auctions_informations` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
DELIMITER ;;
CREATE DEFINER=`giuliano`@`%` PROCEDURE `closed_auctions_informations`()
BEGIN
	SELECT
		`Codice`,
        `Stato`,
        `Lunghezza`,
        `Larghezza`,
        `Altezza`,
        `Descrizione`,
        `BaseAsta`,
        `ScadenzaAsta`,
        `PrimoLivello`,
        `SecondoLivello`,
        `TerzoLivello`,
        `OffertaMassima`,
		`NumeroOfferte`
	FROM Aste
    WHERE ScadenzaAsta<=NOW();
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
/*!50003 DROP PROCEDURE IF EXISTS `insert_category` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
DELIMITER ;;
CREATE DEFINER=`giuliano`@`%` PROCEDURE `insert_category`(IN `primo_livello` VARCHAR(32), IN `secondo_livello` VARCHAR(32), IN `terzo_livello` VARCHAR(32))
BEGIN
	DECLARE EXIT HANDLER FOR SQLEXCEPTION, SQLSTATE '45001'
    BEGIN
    	ROLLBACK;
        RESIGNAL;
    END;
	
    INSERT INTO Categorie(PrimoLivello, SecondoLivello, TerzoLivello) VALUES (primo_livello, secondo_livello, terzo_livello);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
/*!50003 DROP PROCEDURE IF EXISTS `login` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
DELIMITER ;;
CREATE DEFINER=`giuliano`@`%` PROCEDURE `login`(IN var_username varchar(64), IN password varchar(64),  OUT var_role TINYINT(1), OUT var_cf CHAR(16))
BEGIN
	DECLARE user_role ENUM('amministratore', 'utente');
    
    SELECT `Ruolo`
		FROM Auth
		WHERE Auth.username = var_username AND Auth.Password = md5(password) INTO user_role;

		SELECT CF
		FROM Utenti
		WHERE Username = var_username INTO var_cf;

		
		IF user_role = 'amministratore' THEN
			SET var_role = 1;
		ELSEIF user_role = 'utente' THEN
			SET var_role = 2;
		ELSE
			SET var_role = 3;
		END if;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
/*!50003 DROP PROCEDURE IF EXISTS `make_offer` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
DELIMITER ;;
CREATE DEFINER=`giuliano`@`%` PROCEDURE `make_offer`(IN `utente` CHAR(16), IN `importo` FLOAT UNSIGNED, IN `oggetto` CHAR(7))
BEGIN
	DECLARE importo_controfferta FLOAT UNSIGNED;
	DECLARE controfferente CHAR(16);
	DECLARE EXIT HANDLER FOR SQLEXCEPTION, SQLSTATE '45001', SQLSTATE '45002'
    begin
	rollback;
        resignal;
	end;
	set transaction isolation level read committed;
    start transaction;

		SELECT Importo, Utente
        FROM Controfferte
        WHERE Oggetto=oggetto INTO importo_controfferta, controfferente;
        
        IF importo_controfferta >= importo AND importo_controfferta IS NOT NULL THEN
			INSERT INTO Offerte VALUES (importo+0.50, oggetto, controfferente, NOW());
            SIGNAL SQLSTATE '45002' set message_text = "[OfferError] There is a counteroffer higher then your offer!";
		ELSE
			INSERT INTO Offerte VALUES(importo, oggetto, utente, NOW());
		END IF;
	commit;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
/*!50003 DROP PROCEDURE IF EXISTS `set_counteroffer` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
DELIMITER ;;
CREATE DEFINER=`giuliano`@`%` PROCEDURE `set_counteroffer`(IN `importo` FLOAT, IN `utente` CHAR(16), IN `oggetto` CHAR(7))
BEGIN
	DECLARE EXIT HANDLER FOR SQLEXCEPTION, SQLSTATE "45001", SQLSTATE "45002"
    BEGIN
    	ROLLBACK;
    	RESIGNAL;
    END;
    
	INSERT INTO Controfferte VALUES (importo, utente, oggetto);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
/*!50003 DROP PROCEDURE IF EXISTS `user_auctions_informations` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
DELIMITER ;;
CREATE DEFINER=`giuliano`@`%` PROCEDURE `user_auctions_informations`(IN `var_utente` CHAR(16))
BEGIN
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
    	ROLLBACK;
        RESIGNAL;
    END;
    
	SELECT `Codice`, `Descrizione`, ScadenzaAsta, NumeroOfferte, OffertaMassima
    FROM Aste
    WHERE `ScadenzaAsta` > CURRENT_TIMESTAMP() AND Partecipante=var_utente
ORDER BY(ScadenzaAsta);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
/*!50003 DROP PROCEDURE IF EXISTS `user_registration` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
DELIMITER ;;
CREATE DEFINER=`giuliano`@`%` PROCEDURE `user_registration`(IN `username` VARCHAR(32), IN `password` VARCHAR(32), IN `cf` CHAR(16), IN `nome` VARCHAR(32), IN `cognome` VARCHAR(32), IN `indirizzo` VARCHAR(64), IN `data_nascita` DATE, IN `citta_nascita` VARCHAR(32), IN `numero_carta` CHAR(19), IN `cvv` SMALLINT, IN `data_scadenza` DATE)
BEGIN
	declare exit handler for sqlexception, SQLSTATE "45001"
    begin
		rollback;
        resignal;
	end;
	set transaction isolation level read committed; 
    start transaction;
		INSERT INTO Auth VALUES(username, md5(password), 'user');
		INSERT INTO Utenti VALUES(cf, nome, cognome, indirizzo, data_nascita, citta_nascita, username);
		INSERT INTO CarteCredito VALUES (numero_carta, cf, cvv, data_scadenza);
	commit;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Final view structure for view `Aste`
--

/*!50001 DROP VIEW IF EXISTS `Aste`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb3 */;
/*!50001 SET character_set_results     = utf8mb3 */;
/*!50001 SET collation_connection      = utf8mb3_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`giuliano`@`%` SQL SECURITY DEFINER */
/*!50001 VIEW `Aste` AS select `Oggetti`.`Codice` AS `Codice`,`Oggetti`.`Stato` AS `Stato`,`Oggetti`.`Lunghezza` AS `Lunghezza`,`Oggetti`.`Larghezza` AS `Larghezza`,`Oggetti`.`Altezza` AS `Altezza`,`Oggetti`.`Descrizione` AS `Descrizione`,`Oggetti`.`BaseAsta` AS `BaseAsta`,`Oggetti`.`ScadenzaAsta` AS `ScadenzaAsta`,`Oggetti`.`NumeroOfferte` AS `NumeroOfferte`,`Oggetti`.`PrimoLivello` AS `PrimoLivello`,`Oggetti`.`SecondoLivello` AS `SecondoLivello`,`Oggetti`.`TerzoLivello` AS `TerzoLivello`,`MAX_OFFER`(`Oggetti`.`Codice`) AS `OffertaMassima`,`Offerte`.`Utente` AS `Partecipante` from (`Oggetti` left join `Offerte` on((`Oggetti`.`Codice` = `Offerte`.`Oggetto`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2023-02-23 21:33:18
