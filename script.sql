create database db;
use db;
create table `files` (`id` INT NOT NULL AUTO_INCREMENT, `datetime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP, `filename` CHAR(50) NOT NULL, PRIMARY KEY(`id`));
