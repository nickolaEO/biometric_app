import tkinter as tk
from tkinter import messagebox as mbox
from tkinter import ttk

import arduino_transfer_data as atd
from main import *


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('BIOmetrica')
        self.iconbitmap('icon.ico')
        self.geometry('600x500')
        self.resizable(width=False, height=False)
        self.put_menu()
        self.put_frames()
        self.popup = Popup(self)

    def put_menu(self):
        self.config(menu=MainMenu(self))

    def put_frames(self):
        self.info_frame = InfoFrame(self).place(
            relx=0,
            rely=0,
            relwidth=1,
            relheight=0.30
        )
        self.input_frame = InputFrame(self).place(
            relx=0,
            rely=0.30,
            relwidth=1,
            relheight=0.30
        )
        self.table_frame = TableFrame(self).place(
            relx=0,
            rely=0.60,
            relwidth=1,
            relheight=0.35
        )

    def refresh(self):
        all_frames = [f for f in self.children]
        for f_name in all_frames:
            self.nametowidget(f_name).destroy()
        self.put_frames()
        self.put_menu()

    def switch_com_port(self, com_port):
        # self.com_port = com_port
        atd.choose_com_port(com_port)
        self.refresh()


class Popup:
    def __init__(self, master):
        self.master = master

    def show(self, window_type):
        getattr(self, window_type)()

    def quit(self):
        answer = mbox.askyesno('Выход', 'Вы действительно хотите выйти?')
        if answer:
            self.master.destroy()

    def faq(self):
        mbox.showinfo('Инструкция', 'Здесь будет размещена инструкция')

    def about_us(self):
        mbox.showinfo(
            'О Нас',
            'ГБПОУ Политехнический колледж им. Н. Н. Годовикова\n\n'
            'Лаборатория "Проектирование нейроинтерфейсов"\n\n'
            'ул. Клары Цеткин, 23, Москва'
        )


class MainMenu(tk.Menu):
    def __init__(self, mainwindow):
        super().__init__(mainwindow)

        file_menu = tk.Menu(self, tearoff=0)
        options_menu = tk.Menu(self, tearoff=0)
        help_menu = tk.Menu(self, tearoff=0)

        file_menu.add_command(
            label='Обновить окно',
            command=mainwindow.refresh
        )
        file_menu.add_separator()
        file_menu.add_command(
            label='Выход',
            command=lambda: mainwindow.popup.show('quit')
        )

        com_port_menu = tk.Menu(options_menu, tearoff=0)
        self.com_var = tk.StringVar()
        for com_port in atd.serial_ports():
            com_port_menu.add_radiobutton(
                label=com_port,
                variable=self.com_var,
                value=com_port,
                command=lambda: mainwindow.switch_com_port(self.com_var.get())
            )
        options_menu.add_cascade(label='COM порт', menu=com_port_menu)

        help_menu.add_command(
            label='Инструкция',
            command=lambda: mainwindow.popup.show('faq')
        )
        help_menu.add_separator()
        help_menu.add_command(
            label="О Нас",
            command=lambda: mainwindow.popup.show('about_us')
        )

        self.add_cascade(label='Файл', menu=file_menu)
        self.add_cascade(label='Настройки', menu=options_menu)
        self.add_cascade(label='Помощь', menu=help_menu)


class InfoFrame(ttk.Labelframe):
    def __init__(self, container):
        super().__init__(container)
        self.put_widgets()

    def put_widgets(self):
        self.l_info = ttk.Label(
            self,
            text=('Добро пожаловать в BIOmetrica!\n'
                  'Приложите палец к сканеру отпечатка для распознования.\n'
                  'Для того, чтобы попасть в базу, '
                  'введите ФИО в соответсвующие поля,\nнажмите "Добавить" '
                  'и следуйте дальнейшим инструкциям.'),
            justify='center'
        )
        self.l_info.pack(expand=True)


class InputFrame(ttk.Frame):
    def __init__(self, container):
        super().__init__(container)
        self.put_widgets()

    def put_widgets(self):
        self.l_input_lname = ttk.Label(self, text="ФАМИЛИЯ", )
        self.f_input_lname = ttk.Entry(self)
        self.f_input_lname.focus()
        self.l_input_fname = ttk.Label(self, text="ИМЯ")
        self.f_input_fname = ttk.Entry(self)
        self.l_input_ptrnymic = ttk.Label(self, text="ОТЧЕСТВО")
        self.f_input_ptrnymic = ttk.Entry(self)

        self.btn_drop = ttk.Button(
            self,
            width=20,
            text='ОЧИСТИТЬ БАЗУ',
            command=self.clear_table
        )
        self.btn_add = ttk.Button(
            self,
            width=20,
            text='ДОБАВИТЬ',
            command=self.add_person
        )
        self.btn_del = ttk.Button(
            self,
            width=20,
            text='УДАЛИТЬ ИЗ БАЗЫ',
            command=self.delete_person
        )

        self.l_input_lname.grid(row=0, column=0, padx=15)
        self.f_input_lname.grid(row=1, column=0,  padx=15)
        self.l_input_fname.grid(row=0, column=1, padx=15)
        self.f_input_fname.grid(row=1, column=1, padx=15)
        self.l_input_ptrnymic.grid(row=0, column=2, padx=15)
        self.f_input_ptrnymic.grid(row=1, column=2, padx=15)

        self.btn_drop.grid(row=2, column=0, pady=25, ipady=15)
        self.btn_add.grid(row=2, column=1, pady=25, ipady=15)
        self.btn_del.grid(row=2, column=2, pady=25, ipady=15)

    def take_inputed_text(self):
        lname = self.f_input_lname.get()
        fname = self.f_input_fname.get()
        ptrnymic = self.f_input_ptrnymic.get()
        return (
                lname.capitalize() + ' ' +
                fname.capitalize() + ' ' +
                ptrnymic.capitalize()
                )

    def clear_inputed_text(self):
        self.f_input_lname.delete(0, 'end')
        self.f_input_fname.delete(0, 'end')
        self.f_input_ptrnymic.delete(0, 'end')
        self.master.refresh()

    def clear_table(self):
        self.bell()
        answer = mbox.askyesno(
            title='Очистка базы',
            message='Вы действительно хотите очистить базу?'
        )
        if answer:
            # atd.drop_arduino_database()
            clear_person_table()
            self.clear_inputed_text()

    def add_person(self):
        if self.take_inputed_text() == '  ':
            mbox.showerror(
                title='Добавление в базу',
                message='Вы не ввели никаких данных!'
            )
        else:
            add_person_to_db(self.take_inputed_text())
            # id = get_id_by_name(self.take_inputed_text())
            # atd.enroll_person(str(id))
            mbox.showinfo(
                title='Добавление в базу',
                message=(f'"{self.take_inputed_text()}" '
                         'был успешно добавлен в базу'))
        self.clear_inputed_text()

    def delete_person(self):
        if self.take_inputed_text() == '  ':
            mbox.showerror(
                title='Удаление из базы',
                message='Вы не ввели никаких данных!'
            )
        else:
            self.bell()
            answer = mbox.askyesno(
                title='Удаление из базы',
                message=('Вы действительно хотите удалить '
                         f'"{self.take_inputed_text()}" из базы?')
            )
            if answer:
                # id = get_id_by_name(self.take_inputed_text())
                # atd.delete_person(str(id))
                remove_person_from_db(self.take_inputed_text())
                mbox.showinfo(
                    title='Удаление из базы',
                    message=f'"{self.take_inputed_text()}" был удален из базы'
                )
            self.clear_inputed_text()


class TableFrame(ttk.Frame):
    def __init__(self, parent):
        super().__init__(parent)
        self.put_widgets()

    def put_widgets(self):
        table = ttk.Treeview(
            self,
            show='headings',
            columns=['id', 'full_name', 'reg_date']
        )
        table.heading('id', text='id', anchor='center')
        table.column('id', width=80, anchor='center', stretch=False)
        table.heading('full_name', text='ФИО', anchor='center')
        table.column('full_name', anchor='w', stretch=True)
        table.heading('reg_date', text='Дата/время записи', anchor='center')
        table.column('reg_date', anchor='center', stretch=True)

        for row in get_table_data():
            table.insert('', tk.END, values=row)

        scroll_pane = ttk.Scrollbar(self, command=table.yview)
        table.configure(yscrollcommand=scroll_pane.set)
        scroll_pane.pack(side=tk.RIGHT, fill=tk.Y)
        table.pack(expand=tk.YES, fill=tk.BOTH)


if __name__ == "__main__":
    app = App()
    app.mainloop()
