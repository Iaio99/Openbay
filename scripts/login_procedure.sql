CREATE PROCEDURE `login` (IN username varchar(64), IN password varchar(64), OUT role TINYINT(1))
BEGIN
	DECLARE user_role ENUM('admin', 'user');
    
    SELECT Role
	FROM Users
	WHERE Users.Username = username AND Users.Password = md5(password) INTO user_role;
        
	-- See the corresponding enum in the client
		IF user_role = 'amministratore' THEN
			SET role = 1;
		ELSEIF user_role = 'agenzia' THEN
			SET role = 2;
		ELSE
			SET role = 3;
		END if;
END
