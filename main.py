import fastapi
import httpx
from pydantic import BaseModel
from fastapi.middleware.cors import CORSMiddleware

app = fastapi.FastAPI()

class Dispositivo(BaseModel):
    valor: str

# Origins
origins = [
    "http://localhost:8080",
    "http://127.0.0.1:5000",
    "https://herokufrontend23-2e5ad8e49cc5.herokuapp.com",
]

# Cors
app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Rutas para la nueva base de datos en la nube

@app.get("/dispositivos")
async def obtener_dispositivos():
    async with httpx.AsyncClient() as client:
        response = await client.get("https://backend-iot-9945b3a20353.herokuapp.com/dispositivos")
        return response.json()

@app.get("/dispositivos/{id_dispositivo}")
async def obtener_valor_dispositivo(id_dispositivo: int):
    async with httpx.AsyncClient() as client:
        response = await client.get(f"https://backend-iot-9945b3a20353.herokuapp.com/dispositivos/{id_dispositivo}")
        return response.json()

@app.put("/dispositivos/{id_dispositivo}")
async def actualizar_valor_dispositivo(id_dispositivo: int, dispositivo: Dispositivo):
    async with httpx.AsyncClient() as client:
        response = await client.put(
            f"https://backend-iot-9945b3a20353.herokuapp.com/dispositivos/{id_dispositivo}",
            json={"valor": dispositivo.valor}
        )
        return {"mensaje": "Valor actualizado"}