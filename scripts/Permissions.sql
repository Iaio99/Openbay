CREATE USER 'LOGIN' IDENTIFIED BY 'login';
CREATE ROLE 'role_login';
GRANT 'role_login' TO 'LOGIN';
SET DEFAULT ROLE 'role_login' TO 'LOGIN';
    GRANT EXECUTE ON PROCEDURE login TO 'role_login';
GRANT EXECUTE ON PROCEDURE user_registration TO 'role_login';

CREATE USER 'USER' IDENTIFIED BY 'user';
CREATE ROLE 'role_user';
GRANT 'role_user' TO 'USER';
SET DEFAULT ROLE 'role_user' to 'USER';

CREATE USER 'ADMIN' IDENTIFIED BY 'admin';
CREATE ROLE 'role_admin';
GRANT 'role_admin' TO 'ADMIN';
SET DEFAULT ROLE 'role_admin' to 'ADMIN';