-- createMovie.sql
drop table if exists Movie;
CREATE TABLE `Movie` (
  `movieId`         char(9)     NOT NULL,
  `title`           varchar(31) NOT NULL,
  `genre`           varchar(11) DEFAULT 'UNKNOWN',
  `rating`          varchar(4)  DEFAULT 'PG',
  `minutes`         int(4)      DEFAULT 0,
  `releaseDt`       date        DEFAULT NULL,
  PRIMARY KEY (`movieId`)
) ;