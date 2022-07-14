import tkinter as tk
from tkinter import ttk
from tkinter.messagebox import showinfo

class App(tk.Tk):
  def __init__(self):
    super().__init__()

    # configure the root window
    self.title('CANBusTesting')
    self.geometry('300x200')

    # label
    self.label = ttk.Label(self, text='Click start to begin testing')
    self.label.pack()
    print('test')
    # button
    self.button = ttk.Button(self, text='Start')
    self.button['command'] = self.button_clicked
    self.button.pack()

  def button_clicked(self):
    showinfo(title='Information', message='Hello, Tkinter!')

if __name__ == "__main__":
    app = App()
    app.mainloop()
    
