-- MariaDB dump 10.19  Distrib 10.11.2-MariaDB, for Linux (x86_64)
--
-- Host: localhost    Database: Aste_Online
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
  `Username` varchar(64) COLLATE utf8mb4_unicode_ci NOT NULL,
  `Password` varchar(64) COLLATE utf8mb4_unicode_ci NOT NULL,
  `Ruolo` enum('amministratore','utente') COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`Username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `CarteCredito`
--

DROP TABLE IF EXISTS `CarteCredito`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `CarteCredito` (
  `Numero` char(19) COLLATE utf8mb4_unicode_ci NOT NULL,
  `Intestatario` char(16) COLLATE utf8mb4_unicode_ci NOT NULL,
  `CVV` smallint unsigned NOT NULL,
  `DataScadenza` date NOT NULL,
  PRIMARY KEY (`Intestatario`,`Numero`),
  CONSTRAINT `CarteCredito_ibfk_1` FOREIGN KEY (`Intestatario`) REFERENCES `Utenti` (`CF`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
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
  `PrimoLivello` varchar(32) COLLATE utf8mb4_unicode_ci NOT NULL,
  `SecondoLivello` varchar(32) COLLATE utf8mb4_unicode_ci NOT NULL,
  `TerzoLivello` varchar(32) COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`PrimoLivello`,`SecondoLivello`,`TerzoLivello`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
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
  `Oggetto` char(7) COLLATE utf8mb4_unicode_ci NOT NULL,
  `Utente` char(16) COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`Importo`,`Oggetto`),
  KEY `Utente` (`Utente`),
  KEY `Oggetto` (`Oggetto`),
  CONSTRAINT `Controfferte_ibfk_1` FOREIGN KEY (`Utente`) REFERENCES `Utenti` (`CF`),
  CONSTRAINT `Controfferte_ibfk_2` FOREIGN KEY (`Oggetto`) REFERENCES `Oggetti` (`Codice`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
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
  `Oggetto` char(7) COLLATE utf8mb4_unicode_ci NOT NULL,
  `Utente` char(16) COLLATE utf8mb4_unicode_ci NOT NULL,
  `Timestamp` timestamp NOT NULL,
  PRIMARY KEY (`Importo`,`Oggetto`),
  KEY `Utente` (`Utente`),
  KEY `Oggetto` (`Oggetto`),
  CONSTRAINT `Offerte_ibfk_1` FOREIGN KEY (`Utente`) REFERENCES `Utenti` (`CF`),
  CONSTRAINT `Offerte_ibfk_2` FOREIGN KEY (`Oggetto`) REFERENCES `Oggetti` (`Codice`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
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
  `Codice` char(7) COLLATE utf8mb4_unicode_ci NOT NULL,
  `Stato` varchar(32) COLLATE utf8mb4_unicode_ci NOT NULL,
  `Larghezza` tinyint unsigned NOT NULL,
  `Altezza` tinyint unsigned NOT NULL,
  `Lunghezza` tinyint unsigned NOT NULL,
  `Descrizione` text COLLATE utf8mb4_unicode_ci NOT NULL,
  `BaseAsta` float unsigned NOT NULL,
  `ScadenzaAsta` datetime NOT NULL,
  `NumeroOfferte` smallint unsigned NOT NULL,
  `PrimoLivello` varchar(32) COLLATE utf8mb4_unicode_ci NOT NULL,
  `SecondoLivello` varchar(32) COLLATE utf8mb4_unicode_ci NOT NULL,
  `TerzoLivello` varchar(32) COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`Codice`),
  KEY `Oggetti_ibfk_1` (`PrimoLivello`,`SecondoLivello`,`TerzoLivello`),
  KEY `ScadenzaAsta` (`ScadenzaAsta`),
  CONSTRAINT `Oggetti_ibfk_1` FOREIGN KEY (`PrimoLivello`, `SecondoLivello`, `TerzoLivello`) REFERENCES `Categorie` (`PrimoLivello`, `SecondoLivello`, `TerzoLivello`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `Utenti`
--

DROP TABLE IF EXISTS `Utenti`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Utenti` (
  `CF` char(16) COLLATE utf8mb4_unicode_ci NOT NULL,
  `Nome` varchar(32) COLLATE utf8mb4_unicode_ci NOT NULL,
  `Cognome` varchar(32) COLLATE utf8mb4_unicode_ci NOT NULL,
  `Indirizzo` varchar(64) COLLATE utf8mb4_unicode_ci NOT NULL,
  `DataNascita` date NOT NULL,
  `CittaNascita` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Username` varchar(64) COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`CF`),
  KEY `Username` (`Username`),
  CONSTRAINT `Utenti_ibfk_1` FOREIGN KEY (`Username`) REFERENCES `Auth` (`Username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
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

-- Dump completed on 2023-02-23 21:14:42
