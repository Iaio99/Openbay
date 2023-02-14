CREATE FUNCTION OFFERTA_MAX (codice char(7))
RETURNS FLOAT UNSIGNED
BEGIN
	DECLARE numero_offerte SMALLINT UNSIGNED;
    DECLARE base_asta FLOAT UNSIGNED;
    DECLARE offerta_massima FLOAT UNSIGNED;
    
	SELECT NumeroOfferte, BaseAsta, MAX(Offerte.Importo)
    FROM Oggetti JOIN Offerte ON Oggetti.Codice = Offerte.codice
    WHERE Oggetti.Codice = codice INTO numero_offerte, base_asta, offerta_massima;

	IF offerta_massima IS NULL THEN
		return base_asta;
	ELSE
		return offerta_massima;
	END IF;
END