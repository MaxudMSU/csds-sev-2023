from matplotlib import pyplot as plt
from matplotlib.figure import Figure
from tkinter import *
from tkinter import ttk
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
import os
import time

itercount = 1
entire_time = 0

def iterate():
    global canvas
    global mpl_toolbar
    global itercount
    global plot1
    clear_res()
    global entire_time
    par_list = ' '.join(list(map(lambda x: x.get(), entery_list[2:])))
    # print(par_list)
    start = time.time()
    os.system('g++ entropy.cpp')
    os.system('./a.out ' + par_list + ' ' + str(itercount))
    end = time.time()
    iter_time = end - start
    entire_time += iter_time
    with open('file.osip') as file:
        points = file.read()
        splitPoints = points.split()
        floatPoints = list(map(float, splitPoints))
        xs = floatPoints[0::2]
        ys = floatPoints[1::2]

    with open('res.osip') as res:
        res_raw = res.read()
        # split_res = res_raw.split()
    # исправить
    final_res = [res_raw] + [str(iter_time)] + [str(entire_time)] + [str(itercount)]
    fill_res(final_res)
    block_res()
    plot1.clear()
    plot1.grid()
    plt.title(f'Итерация № {itercount}')
    plot1.plot(xs, ys, color='red')

    canvas.draw()
    # plt.ioff()
    itercount += 1

def remove_kebab():
    global canvas
    global mpl_toolbar
    global itercount
    itercount = 1
    if canvas:
        # mpl_toolbar.destroy()
        canvas.get_tk_widget().destroy()
        # canvas.delete('all')
        # plt.clf()
        # fig.clear()
        clear_res()

def fill_map():
    clear_all()
    x_entry.insert(0, '1 + y - a*x**2')
    y_entry.insert(0, 'b*x')
    para_entry.insert(0, '1.4')
    parb_entry.insert(0, '0.3')
    x1_entry.insert(0,'0')
    y1_entry.insert(0, '0')
    x2_entry.insert(0, '1')
    y2_entry.insert(0, '1')

def clear_all():
    for i in entery_list:
        i.delete(0, 'end')

def block_res():
    [x.config(state='readonly') for x in res_entries]

def clear_res():
    for i in res_entries:
        i.config(state='normal')
        i.delete(0, 'end')
    # block_res()

def fill_res(values):
    for i in range(len(values)):
        res_entries[i].insert(0, values[i])

def start():
    global canvas
    global mpl_toolbar
    global plot1
    fig = Figure()
    # plt.ion()
    plot1 = fig.add_subplot(111)
    canvas = FigureCanvasTkAgg(fig, master = frame2)
    canvas.draw()
    toolbar = NavigationToolbar2Tk(canvas, frame2, pack_toolbar=False)
    toolbar.update()
    # plt.ioff()
    canvas.get_tk_widget().pack(expand=True, fill=BOTH)
    toolbar.pack(expand=True, fill=X)

window = Tk()
window.title('Итерация кривой и энтропия')

menu = Menu(window)
window.config(menu=menu)
filemenu = Menu(menu, tearoff=0)
menu.add_cascade(label='File', menu=filemenu)
# filemenu.add_command(label='Очистить', command=clear_all)
mapsmenu = Menu(menu, tearoff=0)
menu.add_cascade(label='Maps', menu=mapsmenu)
mapsmenu.add_command(label='map1', command=fill_map)

notebook = ttk.Notebook()
notebook.pack(expand=True, fill=BOTH)

frame1 = Frame(notebook, padx=10, pady=10)
frame1.pack(expand=True, fill=BOTH)
notebook.add(frame1, text="Ввод данных")

map_label = Label(frame1, text="Выбор отображения")
map_label.pack(fill=BOTH)

frame11 = Frame(frame1, borderwidth=1, relief=SOLID)
frame11.pack(expand=True, fill=BOTH)

frame111 = Frame(frame11)
frame111.pack(side=LEFT, expand=True, fill=BOTH, padx=10, pady=10)

x_label = Label(frame111, text='f(x)= ', anchor='e')
x_label.pack(expand=True, fill=BOTH)

y_label = Label(frame111, text='y(x)= ', anchor='e')
y_label.pack(expand=True, fill=BOTH)

para_label = Label(frame111, text='a= ', anchor='e')
para_label.pack(expand=True, fill=BOTH)

parb_label = Label(frame111, text='b= ', anchor='e')
parb_label.pack(expand=True, fill=BOTH)

acc_label = Label(frame111, text='Точность: ', anchor='e')
acc_label.pack(expand=True, fill=BOTH)


frame112 = Frame(frame11)
frame112.pack(side=LEFT, expand=True, fill=BOTH, padx=10, pady=10)

x_entry = Entry(frame112)
x_entry.pack(expand=True, fill=X)

y_entry = Entry(frame112)
y_entry.pack(expand=True, fill=X)

para_entry = Entry(frame112)
para_entry.pack(expand=True, fill=X)

parb_entry = Entry(frame112)
parb_entry.pack(expand=True, fill=X)

acc_entry = Entry(frame112)
acc_entry.pack(expand=True, fill=X)

points_label = Label(frame1, text="Выбор начального отрезка")
points_label.pack(fill=BOTH)

frame12 = Frame(frame1)
frame12.pack(expand=True, fill=BOTH)

frame121 = Frame(frame12, borderwidth=1, relief=SOLID)
frame121.pack(side=LEFT, expand=True, fill=BOTH, padx=10)

# points_label_1 = Label(frame121, text='Вектор 1')
# points_label_1.pack(expand=True, fill=BOTH)

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

frame122 = Frame(frame12, borderwidth=1, relief=SOLID)
frame122.pack(side=LEFT, expand=True, fill=BOTH, padx=10)

# points_label_2 = Label(frame122, text='Вектор 2')
# points_label_2.pack(expand=True, fill=BOTH)

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

start_btn = Button(frame1, text='Запуск', command=start)
start_btn.pack(pady=10)

frame2 = Frame(notebook)
frame2.pack(expand=True, fill=BOTH, padx=10, pady=10)
notebook.add(frame2, text='Результаты')

frame21 = Frame(frame2)
frame21.pack(expand=True, fill=BOTH)

frame211 = Frame(frame21)
frame211.pack(side=LEFT, expand=True, fill=BOTH)

res2_label = Label(frame211, text="Энтропия >= ", font=("Arial", 14), anchor='e')
res2_label.pack(expand=True, fill=BOTH)

res3_label = Label(frame211, text="Время работы итерации: ", font=("Arial", 14), anchor='e')
res3_label.pack(expand=True, fill=BOTH)

res4_label = Label(frame211, text="Общее ремя работы: ", font=("Arial", 14), anchor='e')
res4_label.pack(expand=True, fill=BOTH)

iter_label = Label(frame211, text="Количество итераций", font=("Arial", 14), anchor='e')
iter_label.pack(expand=True, fill=BOTH)

delete_btn = Button(frame211, text="Очистить результаты", command = remove_kebab)
delete_btn.pack()

frame212 = Frame(frame21)
frame212.pack(side=LEFT, expand=True, fill=BOTH)

res2_value = Entry(frame212)
res2_value.pack(expand=True, fill=X)

res3_value = Entry(frame212)
res3_value.pack(expand=True, fill=X)

res4_value = Entry(frame212)
res4_value.pack(expand=True, fill=X)

iter_value = Entry(frame212)
iter_value.pack(expand=True, fill=X)

iter_btn = Button(frame212, text='Итерировать', command=iterate)
iter_btn.pack()

entery_list = [x_entry,y_entry,x1_entry, y1_entry, x2_entry, y2_entry, para_entry, parb_entry, acc_entry]
res_entries = [res2_value, res3_value, res4_value, iter_value]

window.mainloop()
