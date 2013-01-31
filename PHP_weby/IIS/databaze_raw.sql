-- Adminer 3.6.1 MySQL dump

SET NAMES utf8;
SET foreign_key_checks = 0;
SET time_zone = 'SYSTEM';
SET sql_mode = 'NO_AUTO_VALUE_ON_ZERO';

DROP TABLE IF EXISTS `Certifikat`;
CREATE TABLE `Certifikat` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `jmeno` varchar(128) COLLATE latin2_czech_cs NOT NULL,
  `popis` varchar(128) COLLATE latin2_czech_cs NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=17 DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;


DROP TABLE IF EXISTS `Certifikat_k_testu`;
CREATE TABLE `Certifikat_k_testu` (
  `ID_cert` int(11) NOT NULL,
  `ID_test` int(11) NOT NULL,
  KEY `ID_cert` (`ID_cert`),
  KEY `ID_test` (`ID_test`),
  CONSTRAINT `Certifikat_k_testu_ibfk_4` FOREIGN KEY (`ID_test`) REFERENCES `Test` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `Certifikat_k_testu_ibfk_3` FOREIGN KEY (`ID_cert`) REFERENCES `Certifikat` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;


DROP TABLE IF EXISTS `Letadlo`;
CREATE TABLE `Letadlo` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `typ` text COLLATE latin2_czech_cs NOT NULL,
  `seriovec` varchar(30) COLLATE latin2_czech_cs NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;


DROP TABLE IF EXISTS `Ma_provest`;
CREATE TABLE `Ma_provest` (
  `login_technika` varchar(8) COLLATE latin2_czech_cs NOT NULL,
  `ID_Plany` tinyint(4) NOT NULL,
  KEY `login_technika` (`login_technika`),
  KEY `ID_Plany` (`ID_Plany`),
  CONSTRAINT `Ma_provest_ibfk_5` FOREIGN KEY (`ID_Plany`) REFERENCES `Plany` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `Ma_provest_ibfk_4` FOREIGN KEY (`login_technika`) REFERENCES `Technik` (`login`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;


DROP TABLE IF EXISTS `Plany`;
CREATE TABLE `Plany` (
  `ID` tinyint(4) NOT NULL AUTO_INCREMENT,
  `Hotovo` int(1) NOT NULL,
  `Datum_zadani` date NOT NULL,
  `Datum_ukonceni` date NOT NULL,
  `ID_test` int(11) NOT NULL,
  `ID_letadlo` int(11) NOT NULL,
  `vysledek` varchar(128) COLLATE latin2_czech_cs NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `ID_test` (`ID_test`),
  KEY `ID_letadlo` (`ID_letadlo`),
  CONSTRAINT `Plany_ibfk_4` FOREIGN KEY (`ID_test`) REFERENCES `Test` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `Plany_ibfk_5` FOREIGN KEY (`ID_letadlo`) REFERENCES `Letadlo` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;


DROP TABLE IF EXISTS `Technik`;
CREATE TABLE `Technik` (
  `login` varchar(8) COLLATE latin2_czech_cs NOT NULL,
  `heslo` varchar(128) COLLATE latin2_czech_cs NOT NULL,
  `prava` int(11) NOT NULL,
  `jmeno` varchar(128) COLLATE latin2_czech_cs NOT NULL,
  `prijmeni` varchar(128) COLLATE latin2_czech_cs NOT NULL,
  `tel_cislo` int(9) NOT NULL,
  `RC` int(10) NOT NULL,
  PRIMARY KEY (`login`)
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO `Technik` (`login`, `heslo`, `prava`, `jmeno`, `prijmeni`, `tel_cislo`, `RC`) VALUES
('admin',	'21232f297a57a5a743894a0e4a801fc3',	0,	'admin',	'admin',	0,	0);

DROP TABLE IF EXISTS `Technik_ma_certi`;
CREATE TABLE `Technik_ma_certi` (
  `ID_Technik` varchar(8) COLLATE latin2_czech_cs NOT NULL,
  `ID_Cert` int(11) NOT NULL,
  KEY `ID_Technik` (`ID_Technik`),
  KEY `ID_Cert` (`ID_Cert`),
  CONSTRAINT `Technik_ma_certi_ibfk_4` FOREIGN KEY (`ID_Cert`) REFERENCES `Certifikat` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `Technik_ma_certi_ibfk_3` FOREIGN KEY (`ID_Technik`) REFERENCES `Technik` (`login`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;


DROP TABLE IF EXISTS `Test`;
CREATE TABLE `Test` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `popis` varchar(128) COLLATE latin2_czech_cs NOT NULL,
  `limity` varchar(128) COLLATE latin2_czech_cs NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;


-- 2012-12-02 14:33:54
