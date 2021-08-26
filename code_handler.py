import sqlite3

def get_all_data():
    add_data = []
    with sqlite3.connect('db/database.db') as db:
        db.row_factory = sqlite3.Row
        cursor = db.cursor()
        query = """ SELECT * FROM persons """
        cursor.execute(query)
        all_data = cursor
    return all_data

def get_table_data():
    data = get_all_data()
    return [(i['id'], i['name']) for i in data]

def add_person_to_db(full_name):
    success = False
    with sqlite3.connect('db/database.db') as db:
        cursor = db.cursor()
        query = """ CREATE TABLE IF NOT EXISTS persons(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name TEXT) """
        query1 = """ INSERT INTO persons (name) VALUES (?);"""
        cursor.execute (query)
        cursor.execute(query1, full_name)
        db.commit()
        success = True
    return full_name

def get_id_by_name(full_name):
    result = []
    with sqlite3.connect ('db/database.db') as db:
        cursor = db.cursor ()
        query = """ SELECT id FROM persons WHERE name = ? """
        cursor.execute (query, full_name)
        db.commit ()
        result = list(cursor)
    return int(result[0][0])

def drop_person_table():
    with sqlite3.connect('db/database.db') as db:
        cursor = db.cursor()
        # query = """DELETE FROM persons """
        query = """ DROP TABLE IF EXISTS persons """
        query1 = """ CREATE TABLE IF NOT EXISTS persons(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name TEXT) """
        cursor.execute (query)
        cursor.execute (query1)
        db.commit()

def delete_person_from_table(full_name):
    success = False
    with sqlite3.connect('db/database.db') as db:
        cursor = db.cursor()
        query = """ DELETE from persons WHERE name = ? """
        cursor.execute (query, full_name)
        db.commit()
        success = True
    return success

def find_person_in_db(full_name):
    success = False
    with sqlite3.connect('db/database.db') as db:
        db.row_factory = sqlite3.Row
        cursor = db.cursor()
        query = """ SELECT * FROM persons WHERE name LIKE (?); """
        cursor.execute(query, full_name)
        success = True
    return success