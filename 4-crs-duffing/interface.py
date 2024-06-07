from matplotlib import pyplot as plt
from matplotlib.figure import Figure
from tkinter import *
from tkinter import ttk
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
import os
import time

def help(str):
    if str:
        a, b = str.split(', ')
        return [eval(a), eval(b)]

def true_start():
    global par_list
    global h
    global isStart
    global iter_num
    global times
    isStart = 1
    clear_res()
    h = float(h_entry.get())
    par_list = ' '.join(list(map(lambda x: x.get(), entery_list[2:])))
    iter_num = 0
    times = 0
    os.system('g++ crs-duffing.cpp')
    proga_work()

def proga_work():
    global par_list
    global h
    global times
    global isStart
    global iter_num
    start = time.time()
    current_par_list = par_list + ' ' + str(h) + ' ' + str(isStart)
    os.system('./a.out ' + current_par_list)
    timee = time.time() - start
    times += timee
    iter_num += 1
    res5_value.config(text=str(iter_num))
    res3_value.config(text=str(timee))
    res4_value.config(text=str(times))
    if not isStart: h = h/2
    res6_value.config(text=str(h))
    isStart = 0

def iter():
    num_iter = spinbox.get()
    for i in range(0,int(num_iter)):
        proga_work()

def draw_result():
    global h
    file = open('points_file.osip', 'r')
    raw = file.read()
    file.close()
    splt = raw.split()
    splt1 = list(map(eval, splt))
    res1_value.config(text=str(len(splt1)/2))
    ax.clear()
    for i in range(1, len(splt1),2):
        rectangle = plt.Rectangle((splt1[i-1], splt1[i]), h, h, fc='black',ec="black")
        ax.add_patch(rectangle)
    ax.set_xlim([-2, 2])
    ax.set_ylim([-2, 2])
    ax.grid()
    canvas.draw()

def fill_map():
    clear_all()
    x_entry.insert(0, 'y')
    y_entry.insert(0, '-a*x-b*x**3-k*y+B*cos(omega*t)')
    paralpha_entry.insert(0, '-0.8')
    parbeta_entry.insert(0, '1')
    park_entry.insert(0, '0.25')
    parb_entry.insert(0, '0.3')
    paromega_entry.insert(0, '1')
    x1_entry.insert(0,'-2')
    y1_entry.insert(0, '-2')
    x2_entry.insert(0, '2')
    y2_entry.insert(0, '2')
    h_entry.insert(0, '0.5')

def clear_all():
    for i in entery_list:
        i.delete(0, 'end')

def block_res():
    [x.config(state='readonly') for x in res_entries]

def clear_res():
    for i in res_entries:
        i.config(text='')

def fill_res(values):
    for i in range(len(values)):
        res_entries[i].insert(0, values[i])

window = Tk()
window.title('Цепно-реккурентное множество')

notebook = ttk.Notebook()
notebook.pack(expand=True, fill=BOTH)

frame1 = Frame(notebook, padx=10, pady=10)
frame1.pack(expand=True, fill=BOTH)
notebook.add(frame1, text="Ввод данных")

frame11 = Frame(frame1)
frame11.pack(expand=True, fill=BOTH)

frame111 = LabelFrame(frame11, borderwidth=1, relief=SOLID, text='Отображение')
frame111.pack(side=LEFT, expand=True, fill=BOTH, padx=10, pady=10)

frame111_left = Frame(frame111)
frame111_left.pack(side=LEFT, expand=True, fill=BOTH, padx=10, pady=10)

x_label = Label(frame111_left, text='dx/dt = ', anchor='e')
x_label.pack(expand=True, fill=BOTH)

y_label = Label(frame111_left, text='dy/dt = ', anchor='e')
y_label.pack(expand=True, fill=BOTH)

frame111_right = Frame(frame111)
frame111_right.pack(side=LEFT, expand=True, fill=BOTH, padx=10, pady=10)

x_entry = Entry(frame111_right)
x_entry.pack(expand=True, fill=X)

y_entry = Entry(frame111_right, width=35)
y_entry.pack(expand=True, fill=X)


frame112 = LabelFrame(frame11, borderwidth=1, relief=SOLID, text='Параметры')
frame112.pack(side=LEFT, expand=True, fill=BOTH, padx=10, pady=10)

frame112_left = Frame(frame112)
frame112_left.pack(side=LEFT, expand=True, fill=BOTH)

paralpha_label = Label(frame112_left, text='alpha = ', anchor='e')
paralpha_label.pack(expand=True, fill=BOTH)

parbeta_label = Label(frame112_left, text='beta = ', anchor='e')
parbeta_label.pack(expand=True, fill=BOTH)

park_label = Label(frame112_left, text='k = ', anchor='e')
park_label.pack(expand=True, fill=BOTH)

parb_label = Label(frame112_left, text='B = ', anchor='e')
parb_label.pack(expand=True, fill=BOTH)

paromega_label = Label(frame112_left, text='omega = ', anchor='e')
paromega_label.pack(expand=True, fill=BOTH)

h_label = Label(frame112_left, text='Начальный размер ячейки: ', anchor='e')
h_label.pack(expand=True, fill=BOTH)

frame112_right = Frame(frame112)
frame112_right.pack(side=LEFT, expand=True, fill=BOTH)

paralpha_entry = Entry(frame112_right)
paralpha_entry.pack(expand=True, fill=X)

parbeta_entry = Entry(frame112_right)
parbeta_entry.pack(expand=True, fill=X)

park_entry = Entry(frame112_right)
park_entry.pack(expand=True, fill=X)

parb_entry = Entry(frame112_right)
parb_entry.pack(expand=True, fill=X)

paromega_entry = Entry(frame112_right)
paromega_entry.pack(expand=True, fill=X)

h_entry = Entry(frame112_right)
h_entry.pack(expand=True, fill=X)

points_label = Label(frame1, text="Координаты начальной области")
points_label.pack(fill=BOTH)

frame12 = Frame(frame1)
frame12.pack(expand=True, fill=BOTH)

frame121 = LabelFrame(frame12, borderwidth=1, relief=SOLID, text='Левый нижний угол')
frame121.pack(side=LEFT, expand=True, fill=BOTH, padx=10)

frame1211 = Frame(frame121)
frame1211.pack(side=LEFT, expand=True, fill=BOTH, padx=10, pady=10)

x1_label = Label(frame1211, text='x0 ', anchor='e')
x1_label.pack(expand=True, fill=BOTH)

y1_label = Label(frame1211, text='y0 ', anchor='e')
y1_label.pack(expand=True, fill=BOTH)

frame1212 = Frame(frame121)
frame1212.pack(side=LEFT, expand=True, fill=BOTH, padx=10, pady=10)

x1_entry = Entry(frame1212)
x1_entry.pack(expand=True, fill=X)

y1_entry = Entry(frame1212)
y1_entry.pack(expand=True, fill=X)

frame122 = LabelFrame(frame12, borderwidth=1, relief=SOLID, text='Правый верхний угол')
frame122.pack(side=LEFT, expand=True, fill=BOTH, padx=10)

frame1221 = Frame(frame122)
frame1221.pack(side=LEFT, expand=True, fill=BOTH, padx=10, pady=10)

x2_label = Label(frame1221, text='x1 ', anchor='e')
x2_label.pack(expand=True, fill=BOTH)

y2_label = Label(frame1221, text='y1 ', anchor='e')
y2_label.pack(expand=True, fill=BOTH)

frame1222 = Frame(frame122)
frame1222.pack(side=LEFT, expand=True, fill=BOTH, padx=10, pady=10)

x2_entry = Entry(frame1222)
x2_entry.pack(expand=True, fill=X)

y2_entry = Entry(frame1222)
y2_entry.pack(expand=True, fill=X)

frame13 = Frame(frame1, borderwidth=1, relief=SOLID)
frame13.pack()

frame2 = Frame(notebook)
frame2.pack(expand=True, fill=BOTH, padx=10, pady=10)
notebook.add(frame2, text='Результаты')

frame21 = Frame(frame2)
frame21.pack(expand=True, fill=BOTH)

frame211 = Frame(frame21)
frame211.pack(side=LEFT, expand=True, fill=BOTH)

res1_label = Label(frame211, text="Количество ячеек: ", font=("Arial", 14), anchor='e')
res1_label.pack(expand=True, fill=BOTH)

res5_label = Label(frame211, text="Количество итераций:  ", font=("Arial", 14), anchor='e')
res5_label.pack(expand=True, fill=BOTH)

res3_label = Label(frame211, text="Время работы итерации: ", font=("Arial", 14), anchor='e')
res3_label.pack(expand=True, fill=BOTH)

res4_label = Label(frame211, text="Общее ремя работы: ", font=("Arial", 14), anchor='e')
res4_label.pack(expand=True, fill=BOTH)

res6_label = Label(frame211, text='Диаметр ячейки: ', font=("Arial", 14), anchor='e')
res6_label.pack(expand=True, fill=BOTH)

# delete_btn = Button(frame211, text="Очистить результаты")
#                     # , command = remove_kebab)
# delete_btn.pack()

frame212 = Frame(frame21)
frame212.pack(side=LEFT, expand=True, fill=BOTH)

res1_value = Label(frame212, text='', anchor='w', font=("Arial", 14))
res1_value.pack(expand=True, fill=BOTH)

res5_value = Label(frame212, text='', anchor='w', font=("Arial", 14))
res5_value.pack(expand=True, fill=BOTH)

res3_value = Label(frame212, text='', anchor='w', font=("Arial", 14))
res3_value.pack(expand=True, fill=BOTH)

res4_value = Label(frame212, text='', anchor='w', font=("Arial", 14))
res4_value.pack(expand=True, fill=BOTH)

res6_value = Label(frame212, text='', anchor='w', font=("Arial", 14))
res6_value.pack(expand=True, fill=BOTH)

iter_frame = Frame(frame2)
iter_frame.pack(expand=True, fill=BOTH)

iter_label = Label(iter_frame, text='Сколько итераций делать за один раз: ', font=("Arial", 14), anchor='e')
iter_label.pack(side=LEFT, expand=True, fill=BOTH)

spinbox_var = StringVar(value=1)
spinbox = ttk.Spinbox(iter_frame, from_=1.0, to=5.0, textvariable=spinbox_var)
spinbox.pack(side=LEFT, expand=True, fill=X)

start_btn = Button(frame2, text='Выполнить первую итерацию', command=true_start)
start_btn.pack(pady=10)

iter_btn = Button(frame2, text='Итерировать', command=iter)
iter_btn.pack(pady=10)

draw_btn = Button(frame2, text="Показать результат", command=draw_result)
draw_btn.pack(pady=10)

fig = Figure()
ax = fig.add_subplot(111)
canvas = FigureCanvasTkAgg(fig, master = frame2)
canvas.draw()
toolbar = NavigationToolbar2Tk(canvas, frame2, pack_toolbar=False)
toolbar.update()
canvas.get_tk_widget().pack(expand=True, fill=BOTH)
toolbar.pack(expand=True, fill=X)

entery_list = [x_entry,y_entry,x1_entry, y1_entry, x2_entry, y2_entry, paralpha_entry, parbeta_entry, park_entry, parb_entry, paromega_entry]
res_entries = [res1_value, res5_value, res3_value, res4_value, res6_value]

fill_map()

window.mainloop()
