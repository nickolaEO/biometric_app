from peewee import *

db = SqliteDatabase('db/database.db')


class Person(Model):
    id = PrimaryKeyField(unique=True)
    name = CharField()
    reg_date = DateTimeField()

    class Meta:
        database = db
        ordering = 'id'
        db_table = 'persons'
