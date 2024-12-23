import os
import sys
import serial
import threading
from datetime import datetime
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QLabel, QVBoxLayout, QTableWidget, QTableWidgetItem,
    QWidget, QPushButton, QHBoxLayout, QMessageBox
)
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import Qt, pyqtSignal, QObject


class WarningHandler(QObject):
    warning_signal = pyqtSignal(float)

    def __init__(self):
        super().__init__()

    def emit_warning(self, temperature):
        """Emit a warning signal with the temperature."""
        self.warning_signal.emit(temperature)


class SmartHomeApp(QMainWindow):
    def __init__(self):
        super().__init__()

        self.lamp_on = False
        self.plug_on = False
        self.entry_counter = 0
        self.temp_warning_threshold = 27
        self.temp_exceeded = False 

        self.warning_handler = WarningHandler()
        self.warning_handler.warning_signal.connect(self.show_temperature_warning)


        self.setWindowTitle("Smart Home Control")
        # self.setWindowIcon(QIcon("utils/app_icon.png"))
        self.setGeometry(100, 100, 800, 600)

        self.setStyleSheet("""
            QMainWindow {
                background-color:rgb(44, 44, 44);
                color: #bbbbbb;
                font-family: "Segoe UI", sans-serif;
            }
            QLabel {
                font-size: 16px;
                color: #bbbbbb;
            }
            QPushButton {
                background-color: #1f1f1f;
                color: #bbbbbb;
                border-radius: 12px;
                padding: 20px 10px;
                font-size: 20px;
                border: 1px solid #333;
                margin: 10px 0;
            }
            QPushButton:hover {
                background-color: #ffffff;
                color: #000000;
            }
            QTableWidget {
                background-color: #1f1f1f;
                color: #ffffff;
                border-radius: 8px;
                font-size: 14px;
                gridline-color: #333333;
            }
            QTableWidget QHeaderView {
                background-color:rgb(31, 31, 31);
                color:rgb(0, 0, 0);
                font-size: 16px;
            }
            QMessageBox {
                background-color: #2b2b2b;
                color: #ffffff;
                font-size: 14px;
            }
        """)

        self.temp_label = QLabel("Temperature: -- °C")
        self.door_status_label = QLabel("Door Status: --")
        self.counter_label = QLabel("Log Entries: 0")

        self.log_table = QTableWidget()
        self.log_table.setColumnCount(2)
        self.log_table.setHorizontalHeaderLabels([ "Timestamp", "Door Status"])
        self.log_table.horizontalHeader().setStretchLastSection(True)
        self.log_table.horizontalHeader().setSectionResizeMode(0, 1)

        self.lamp_button = QPushButton("Toggle Lamp")
        self.lamp_button.clicked.connect(self.toggle_lamp)

        self.plug_button = QPushButton("Toggle Plug")
        self.plug_button.clicked.connect(self.toggle_plug)

        # Layouts
        control_layout = QVBoxLayout()
        control_layout.addWidget(self.temp_label)
        control_layout.addWidget(self.door_status_label)
        control_layout.addWidget(self.counter_label)
        control_layout.addWidget(self.lamp_button)
        control_layout.addWidget(self.plug_button)

        table_layout = QVBoxLayout()
        table_layout.addWidget(self.log_table)

        main_layout = QHBoxLayout()
        main_layout.addLayout(control_layout, 1)
        main_layout.addLayout(table_layout, 2)

        container = QWidget()
        container.setLayout(main_layout)
        self.setCentralWidget(container)

        self.uart_port = serial.Serial(port="COM4", baudrate=9600, timeout=0.1)
        self.start_uart_listener()

    def start_uart_listener(self):
        """Start a thread to listen for UART data."""
        self.uart_thread = threading.Thread(target=self.uart_listener, daemon=True)
        self.uart_thread.start()

    def uart_listener(self):
        """Read data from UART and update the GUI."""
        while True:
            if self.uart_port.in_waiting > 0:
                data = self.uart_port.readline().decode("utf-8").strip()
                self.handle_uart_data(data)

    def handle_uart_data(self, data):
        """Process UART data received from the TivaC."""
        if data.startswith("Door :"):
            status = data.split(":")[1]
            self.update_door_status(status)
        elif data.startswith("Temp :"):
            temperature = float(data.split(":")[1])
            self.update_temperature(temperature)
        # elif data.startwith("Plug :"):
        #     plug_status = float(data.split(":")[1])
        #     self.update_plug_status(plug_status)
        # elif data.startwith ("Lamp :"):
        #     lamp_status = data.split(":")[1]
        #     self.update_lamp_status(lamp_status)
            

    def update_door_status(self, status):
        """Update the door status and log the event."""
        timestamp = datetime.now().strftime("%d-%m-%Y  %H:%M:%S")
        self.entry_counter += 1
        self.door_status_label.setText(f"Door Status: {status}")
        self.counter_label.setText(f"Log Entries: {self.entry_counter}")

        row_position = self.log_table.rowCount()
        self.log_table.insertRow(row_position)
        self.log_table.setItem(row_position, 0, QTableWidgetItem(timestamp))
        self.log_table.setItem(row_position, 1, QTableWidgetItem(status))

    def update_temperature(self, temperature):
        """Update the temperature display and trigger a warning if it exceeds the threshold."""
        self.temp_label.setText(f"Temperature: {temperature:.1f} °C")
        if temperature > self.temp_warning_threshold and not self.temp_exceeded:
            self.temp_exceeded = True
            self.warning_handler.emit_warning(temperature)
        elif temperature <= self.temp_warning_threshold:
            self.temp_exceeded = False

    def show_temperature_warning(self, temperature):
        """Display a warning popup asynchronously."""
        warning_msg = QMessageBox(self)
        warning_msg.setIcon(QMessageBox.Warning)
        warning_msg.setWindowTitle("Temperature Warning")
        warning_msg.setText(f"Warning! The temperature has exceeded safe levels: {temperature:.1f} °C")
        warning_msg.exec_()

    def toggle_lamp(self):
        """Toggle lamp state and update the visual."""
        if self.lamp_on:
            self.uart_port.write("C\n".encode("utf-8"))
            self.lamp_on = False
        else:
            self.uart_port.write("L\n".encode("utf-8"))
            self.lamp_on = True

    def toggle_plug(self):
        """Toggle plug state and update the visual."""
        if self.plug_on:
            self.uart_port.write("O\n".encode("utf-8"))
            self.plug_on = False
        else:
            self.uart_port.write("I\n".encode("utf-8"))
            self.plug_on = True


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = SmartHomeApp()
    window.show()
    sys.exit(app.exec_())
