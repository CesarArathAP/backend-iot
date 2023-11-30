import fastapi
import sqlite3
from pydantic import BaseModel
from fastapi.middleware.cors import CORSMiddleware
import requests  # Agregamos la importación para manejar solicitudes HTTP

# Crea la base de datos
conn = sqlite3.connect("dispositivos.db")

app = fastapi.FastAPI()

class Contacto(BaseModel):
    dispositivo: str
    valor: str
    
class Dispositivo(BaseModel):
    valor: str

# Origins
origins = [
    "http://localhost:8080",
    "http://127.0.0.1:5000",
    ##"https://herokufrontend23-2e5ad8e49cc5.herokuapp.com"
]

# Cors
app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/dispositivos")
async def obtener_dispositivos():
    """Obtiene todos los dispositivos."""
    c = conn.cursor()
    c.execute('SELECT * FROM dispositivos;')  # Cambia la consulta para seleccionar los dispositivos
    response = []
    for row in c:
        dispositivo = {"id_dispositivo": row[0], "dispositivo": row[1], "valor": row[2]}
        response.append(dispositivo)
    return response

@app.get("/dispositivos/{id_dispositivo}")
async def obtener_valor_dispositivo(id_dispositivo: int):
    """Obtiene el valor de un dispositivo por su id_dispositivo."""
    c = conn.cursor()
    c.execute('SELECT valor FROM dispositivos WHERE id_dispositivo = ?', (id_dispositivo,))
    valor = c.fetchone()  # Obtiene la primera fila de la consulta
    return {"valor": valor[0] if valor else None}  # Devuelve el valor o None si no se encontró el dispositivo

@app.put("/dispositivos/{id_dispositivo}/{nuevo_valor}")
async def actualizar_valor_dispositivo(id_dispositivo: int, nuevo_valor: str):
    """Actualiza el valor de un dispositivo por su id_dispositivo."""
    c = conn.cursor()
    c.execute('UPDATE dispositivos SET valor = ? WHERE id_dispositivo = ?', (nuevo_valor, id_dispositivo))
    conn.commit()
    
    # Después de actualizar en la base de datos local, también actualiza en Firebase
    actualizar_firebase(id_dispositivo, nuevo_valor)
    
    return {"mensaje": "Valor actualizado"}

def actualizar_firebase(id_dispositivo: int, nuevo_valor: str):
    # Define la URL de la base de datos Firebase
    url_firebase = f"https://iot-esp32-a458a-default-rtdb.firebaseio.com/iot/{id_dispositivo}/valor.json"

    # Realiza la solicitud PUT para actualizar el valor en Firebase
    requests.put(url_firebase, json=nuevo_valor)