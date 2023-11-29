CREATE TABLE dispositivos (
    id_dispositivo INTEGER PRIMARY KEY AUTOINCREMENT,
    dispositivo TEXT,
    valor TEXT
);

INSERT INTO dispositivos (dispositivo, valor) VALUES ('LED', '0');
INSERT INTO dispositivos (dispositivo, valor) VALUES ('Potenciometro', '1024');
