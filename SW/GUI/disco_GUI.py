#  disco_GUI.py
#  project : Rainette
#  author : Damien Pomelli 
#  last modified : 2025-07-24

import sys
import asyncio
from asyncio import run_coroutine_threadsafe
from bleak import BleakClient, BleakScanner
from PySide6.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, QLabel,
                               QLineEdit, QPushButton, QGroupBox, QMessageBox, QGridLayout)
from PySide6.QtCore import QThread, Signal, Slot

DEVICE_NAME = "DISCO"

def get_address_from_name(name):
    async def _find():
        devices = await BleakScanner.discover(timeout=10.0)
        for d in devices:
            if d.name == name:
                return d.address
        return None
    return asyncio.run(_find())

BLE_DEVICE_ADDRESS = get_address_from_name(DEVICE_NAME)
if BLE_DEVICE_ADDRESS is None:
    raise RuntimeError(f"Périphérique \"{DEVICE_NAME}\" introuvable.")

UUID_BASE = "0000{:04x}-0000-1000-8000-00805f9b34fb"

CHAR_NAMES = {
    0x0021: "START_MEAS",
    0x0022: "END_OF_COMM",
    0x0023: "PING_PONG",
    0x0033: "TEMP",
    0x0035: "ADC_TEST",
    0x0045: "LED_R",
    0x0046: "LED_G",
    0x0047: "LED_B",
    0x0048: "BUTTON",
}

RW_CHARS = {
    0x0021,
    0x0022,
    0x0023,
    0x0045,
    0x0046,
    0x0047
}

SERVICES = {
    "General": [0x0021, 0x0022, 0x0023],
    "Analog": [0x0033, 0x0035],
    "Digital": [0x0045, 0x0046, 0x0047, 0x0048]
}

class BLEWorker(QThread):
    value_updated = Signal(str, str)
    connection_lost = Signal()

    def __init__(self, address, loop):
        super().__init__()
        self.address = address
        self.loop = loop
        self.client = BleakClient(self.address, loop=self.loop)
        self.running = True

    async def notification_handler(self, uuid, sender, data):
        if len(data) == 1:
            val = str(int.from_bytes(data, byteorder='little'))
        elif len(data) == 2:
            val = str(int.from_bytes(data, byteorder='little'))
        else:
            val = data.hex()
        self.value_updated.emit(uuid, val)

    async def notify_loop(self):
        try:
            await self.client.connect()
        except Exception:
            pass

        for group in SERVICES.values():
            for short_uuid in group:
                full_uuid = UUID_BASE.format(short_uuid)
                try:
                    val = await self.client.read_gatt_char(full_uuid)
                    if len(val) == 1:
                        v = str(int.from_bytes(val, byteorder='little'))
                    elif len(val) == 2:
                        v = str(int.from_bytes(val, byteorder='little'))
                    else:
                        v = val.hex()
                    self.value_updated.emit(full_uuid, v)
                except Exception:
                    pass

        for group in SERVICES.values():
            for short_uuid in group:
                full_uuid = UUID_BASE.format(short_uuid)
                try:
                    await self.client.start_notify(
                        full_uuid,
                        lambda sender, data, u=full_uuid: asyncio.create_task(self.notification_handler(u, sender, data))
                    )
                except Exception:
                    pass

        while self.running:
            await asyncio.sleep(0.1)

        for group in SERVICES.values():
            for short_uuid in group:
                full_uuid = UUID_BASE.format(short_uuid)
                try:
                    await self.client.stop_notify(full_uuid)
                except Exception:
                    pass
        await self.client.disconnect()

    def run(self):
        asyncio.set_event_loop(self.loop)
        try:
            self.loop.run_until_complete(self.notify_loop())
        finally:
            self.loop.close()

    def stop(self):
        self.running = False

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("BLE GATT Client")
        self.central = QWidget()
        self.layout = QVBoxLayout()
        self.fields = {}

        for service_name, char_list in SERVICES.items():
            group = QGroupBox(service_name)
            grid = QGridLayout()
            row = 0
            for short_uuid in char_list:
                uuid = UUID_BASE.format(short_uuid)
                name = CHAR_NAMES.get(short_uuid, f"0x{short_uuid:04x}")
                label = QLabel(name)
                read_field = QLineEdit()
                read_field.setReadOnly(True)
                read_field.setFixedWidth(40)

                grid.addWidget(label, row, 0)
                grid.addWidget(read_field, row, 1)

                if short_uuid in RW_CHARS:
                    write_button = QPushButton("Write")
                    write_button.setFixedWidth(50)
                    write_field = QLineEdit()
                    write_field.setFixedWidth(30)
                    def make_write_callback(u=uuid, f=write_field):
                        def cb():
                            self.write_value(u, f.text())
                            f.clear()
                        return cb
                    write_button.clicked.connect(make_write_callback())
                    grid.addWidget(write_button, row, 2)
                    grid.addWidget(write_field, row, 3)
                    self.fields[uuid] = (label, read_field, write_field, write_button)
                else:
                    spacer_button = QWidget()
                    spacer_button.setFixedWidth(50)
                    spacer_field = QWidget()
                    spacer_field.setFixedWidth(30)
                    grid.addWidget(spacer_button, row, 2)
                    grid.addWidget(spacer_field, row, 3)
                    self.fields[uuid] = (label, read_field, None, None)
                row += 1

            group.setLayout(grid)
            self.layout.addWidget(group)

        self.central.setLayout(self.layout)
        self.setCentralWidget(self.central)

        self.setStyleSheet("""
            QMainWindow, QWidget, QGroupBox {
                background-color: #23272e;
                color: #f8f8f2;
            }
            QLabel {
                background-color: transparent;
                color: #f8f8f2;
            }
            QLineEdit {
                background-color: #2c313c;
                color: #f8f8f2;
                border: 1px solid #44475a;
            }
            QPushButton {
                background-color: #44475a;
                color: #f8f8f2;
                border: 1px solid #6272a4;
                padding: 4px 10px;
                border-radius: 4px;
            }
            QPushButton:hover {
                background-color: #6272a4;
            }
        """)

        self.loop = asyncio.new_event_loop()
        self.ble = BLEWorker(BLE_DEVICE_ADDRESS, self.loop)
        self.ble.value_updated.connect(self.update_value)
        self.ble.connection_lost.connect(self.handle_connection_lost)
        self.ble.start()

    @Slot()
    def handle_connection_lost(self):
        self.close()

    @Slot(str, str)
    def update_value(self, uuid, val):
        if uuid in self.fields:
            _, read_field, _, _ = self.fields[uuid]
            analog_uuids = [UUID_BASE.format(0x0033), 
                            UUID_BASE.format(0x0035)]
            try:
                numeric_val = int(val)
                if uuid in analog_uuids:
                    read_field.setText(f"{numeric_val / 100:.2f}")
                else:
                    read_field.setText(str(numeric_val))
            except ValueError:
                read_field.setText(val)

    def write_value(self, uuid, val):
        val = val.strip()
        if not val:
            return
        if not val.isdigit():
            QMessageBox.warning(self, "Erreur d'écriture", "Veuillez entrer un nombre entier.")
            return
        async def write():
            try:
                if not self.ble.client.is_connected:
                    await self.ble.client.connect()
                intval = int(val)
                data = intval.to_bytes(1 if intval < 256 else 2, byteorder='little')
                await self.ble.client.write_gatt_char(uuid, data)
            except Exception as e:
                QMessageBox.warning(self, "Erreur d'écriture", str(e))
        run_coroutine_threadsafe(write(), self.loop)

    def closeEvent(self, event):
        self.ble.stop()
        self.loop.call_soon_threadsafe(self.loop.stop)
        event.accept()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())
