-- createRental.sql
drop table if exists `Rental`;
CREATE TABLE `Rental` (
  `custNr`          smallint    NOT NULL,
  `movieId`         char(9)     NOT NULL,
  `dueDt`           date        DEFAULT NULL,
  PRIMARY KEY (`custNr`,`movieId`)
) ;
