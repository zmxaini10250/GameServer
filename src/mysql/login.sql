CREATE TABLE `login` (
    `idlogin` int(11) NOT NULL AUTO_INCREMENT,
    `username` varchar(45) NOT NULL,
    `password` varchar(45) NOT NULL,
    PRIMARY KEY (`idlogin`),
    UNIQUE KEY `username_UNIQUE` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1


CREATE DEFINER=`root`@`%` FUNCTION `LoginCheck`(username varchar(45), userpassword varchar(45)) RETURNS int(11)
BEGIN
    DECLARE idlogin int default 0;
    select Game.login.idlogin into idlogin from Game.login where Game.login.username = username and Game.login.userpassword = userpassword;
    RETURN idlogin;
END
