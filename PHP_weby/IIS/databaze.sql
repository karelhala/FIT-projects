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
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO `Certifikat` (`ID`, `jmeno`, `popis`) VALUES
(11,	'ISO 4587',	'Prohlídka motorů Boeing'),
(12,	'ISO 4588',	'Test brzd Airbusu'),
(13,	'ISO 1223',	'Prohldka svtel'),
(15,	'vyhláška 50',	'Vyhláška 50 paragraf 6');

DROP TABLE IF EXISTS `Certifikat_k_testu`;
CREATE TABLE `Certifikat_k_testu` (
  `ID_cert` int(11) NOT NULL,
  `ID_test` int(11) NOT NULL,
  KEY `ID_cert` (`ID_cert`),
  KEY `ID_test` (`ID_test`),
  CONSTRAINT `Certifikat_k_testu_ibfk_4` FOREIGN KEY (`ID_test`) REFERENCES `Test` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `Certifikat_k_testu_ibfk_3` FOREIGN KEY (`ID_cert`) REFERENCES `Certifikat` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO `Certifikat_k_testu` (`ID_cert`, `ID_test`) VALUES
(15,	10),
(15,	9),
(11,	8),
(12,	9),
(15,	11),
(11,	11);

DROP TABLE IF EXISTS `Letadlo`;
CREATE TABLE `Letadlo` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `typ` text COLLATE latin2_czech_cs NOT NULL,
  `seriovec` varchar(30) COLLATE latin2_czech_cs NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO `Letadlo` (`ID`, `typ`, `seriovec`) VALUES
(9,	'Boeing 748',	'123456'),
(10,	'Cesna',	'222222'),
(11,	'Airbus',	'111111');

DROP TABLE IF EXISTS `Ma_provest`;
CREATE TABLE `Ma_provest` (
  `login_technika` varchar(8) COLLATE latin2_czech_cs NOT NULL,
  `ID_Plany` tinyint(4) NOT NULL,
  KEY `login_technika` (`login_technika`),
  KEY `ID_Plany` (`ID_Plany`),
  CONSTRAINT `Ma_provest_ibfk_5` FOREIGN KEY (`ID_Plany`) REFERENCES `Plany` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `Ma_provest_ibfk_4` FOREIGN KEY (`login_technika`) REFERENCES `Technik` (`login`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO `Ma_provest` (`login_technika`, `ID_Plany`) VALUES
('blakbr0',	27),
('blakbr0',	28),
('blakbr0',	26),
('hjeklu0',	26),
('halaka0',	29),
('blakbr0',	30),
('halaka0',	30);

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

INSERT INTO `Plany` (`ID`, `Hotovo`, `Datum_zadani`, `Datum_ukonceni`, `ID_test`, `ID_letadlo`, `vysledek`) VALUES
(26,	0,	'2012-12-02',	'0000-00-00',	8,	9,	''),
(27,	0,	'2012-12-02',	'0000-00-00',	11,	10,	''),
(28,	0,	'2012-12-02',	'0000-00-00',	9,	11,	''),
(29,	1,	'2012-12-02',	'2012-12-02',	10,	10,	'Světla svítí'),
(30,	0,	'2012-12-02',	'0000-00-00',	10,	10,	'');

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
('aaaaah0',	'202cb962ac59075b964b07152d234b70',	100,	'ahhhhččč',	'aaaadččč',	0,	0),
('admin',	'21232f297a57a5a743894a0e4a801fc3',	0,	'admin',	'admin',	0,	0),
('blakbr0',	'202cb962ac59075b964b07152d234b70',	20,	'Branislav',	'Blaškovič',	123456789,	1234567890),
('halaka0',	'202cb962ac59075b964b07152d234b70',	10,	'Karel',	'Hala',	123456789,	1234567890),
('hjeklu0',	'202cb962ac59075b964b07152d234b70',	10,	'Lukáš',	'Hájíček',	123456789,	1234567890),
('ortkoo0',	'202cb962ac59075b964b07152d234b70',	100,	'oooooož',	'oártklj',	0,	0);

DROP TABLE IF EXISTS `Technik_ma_certi`;
CREATE TABLE `Technik_ma_certi` (
  `ID_Technik` varchar(8) COLLATE latin2_czech_cs NOT NULL,
  `ID_Cert` int(11) NOT NULL,
  KEY `ID_Technik` (`ID_Technik`),
  KEY `ID_Cert` (`ID_Cert`),
  CONSTRAINT `Technik_ma_certi_ibfk_4` FOREIGN KEY (`ID_Cert`) REFERENCES `Certifikat` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `Technik_ma_certi_ibfk_3` FOREIGN KEY (`ID_Technik`) REFERENCES `Technik` (`login`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO `Technik_ma_certi` (`ID_Technik`, `ID_Cert`) VALUES
('blakbr0',	11),
('blakbr0',	15),
('blakbr0',	12),
('blakbr0',	13),
('hjeklu0',	11),
('halaka0',	15);

DROP TABLE IF EXISTS `Test`;
CREATE TABLE `Test` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `popis` varchar(128) COLLATE latin2_czech_cs NOT NULL,
  `limity` varchar(128) COLLATE latin2_czech_cs NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO `Test` (`ID`, `popis`, `limity`) VALUES
(8,	'Kontrola motorů',	'Vrtule musí fungovat'),
(9,	'Kontrola brzd',	''),
(10,	'Test světel',	'světla musí svítit'),
(11,	'Test elekttrinstalace',	'Nesmí kopat');

-- 2012-12-02 12:47:53
