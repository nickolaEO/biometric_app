import datetime as dt

from models import *


def create_person_table():
    with db:
        db.create_tables([Person])


def delete_person_table():
    # Person.drop_table(safe=True)
    with db:
        db.drop_tables([Person])


def clear_person_table():
    delete_person_table()
    create_person_table()


def get_table_data():
    all_data = [person for person in Person.select()]
    return [
        (i.id, i.name, i.reg_date.strftime('%d.%m.%Y %H:%M:%S'))
        for i in all_data
    ]


def add_person_to_db(full_name):
    # person = Person.create(name=full_name)
    Person.insert(name=full_name, reg_date=dt.datetime.now()).execute()


def get_id_by_name(full_name):
    id_person = Person.select(Person.id).where(Person.name == full_name).get()
    return id_person


def remove_person_from_db(full_name):
    # Person.delete().where(Person.name == full_name).execute()
    person = Person.select().where(Person.name == full_name).get_or_none()
    if person is None:
        return print(f'Человека "{full_name}" не существует')
    else:
        person.delete_instance()
