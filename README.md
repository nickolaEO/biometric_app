# ![BIOmetrica](https://raw.githubusercontent.com/nickolaEO/biometric_app/main/biometrica/icon.ico)**BIO**metrica
## _The application for store and detection fingerprints of a person_

_BIOmetrica_ is a Windows OS Desktop application, which interacts with a microcontroller Arduino + fingerprint sensor.

- Input your **full name** to the fields
- Press **"Add Person"** button to registrate your fingerprint
- Follow the instructions
- Check the addition by placing the finger you added earlier
- If you want to delete the person from databse input **full name** and press **"Delete Person"** button
- If you want to clear database press **"Clead Database"** button

## Tech

- [Python 3.8.8](https://www.python.org/downloads/release/python-388/)
- [TKinter](https://docs.python.org/3/library/tkinter.html) - is a standart Python binding to the Tk GUI toolkit
- [Peewee](http://docs.peewee-orm.com/en/latest/) - is a simple and small ORM

## Installation

OS Windows: 

- Install and activate the virtual environment (venv)

```sh
cd biometric_app
python -m venv venv
venv\Scripts\activate.bat
```

- Install the dependencies from requirements.txt

```sh
cd biometric_app
pip install -r requirements.txt
```

## License

MIT

**Free Software**
