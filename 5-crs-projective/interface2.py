from matplotlib import pyplot as plt
from matplotlib.figure import Figure
from tkinter import *
from tkinter import ttk
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
import numpy as np
from numpy.linalg import eig
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
    h = 1
    str_matrix = matrix_box.get(1.0,END).rstrip()
    par_list = str_matrix.split()
    real_matrix = eval('[[' + str_matrix.replace(' ', ',').replace('\n', '],[') + ']]')
    res7_value.delete(0, 'end')
    res7_value.insert(0, str(eig(np.array(real_matrix))[0]))
    iter_num = 1
    times = 0
    os.system('g++ crs-projective-2.cpp')
    proga_work()

def proga_work():
    global par_list
    global h
    global times
    global isStart
    global iter_num
    start = time.time()
    current_par_list = ' '.join(par_list) + ' ' + str(iter_num)
    # print(current_par_list)
    os.system('./a.out ' + current_par_list)
    timee = time.time() - start
    times += timee
    res5_value.config(text=str(iter_num))
    iter_num += 1
    res3_value.config(text=str(timee))
    res4_value.config(text=str(times))
    if not isStart: h = h/2
    res6_value.config(text=str(h))
    isStart = 0

def iter():
    # num_iter = spinbox.get()
    # for i in range(0,int(num_iter)):
    proga_work()

def draw_result():
    global h
    file = open('points_file.osip', 'r')
    raw = file.read()
    file.close()
    splt = raw.split()
    splt1 = list(map(eval, splt))
    res1_value.config(text=str(len(splt1)/3))
    ax1.clear()
    ax2.clear()
    ax3.clear()
    for i in range(1, len(splt1),3):
        # if splt1[i-1]==1:
        match splt1[i-1]:
            case 1:
                rectangle = plt.Rectangle((splt1[i], splt1[i+1]), h, h, fc='black',ec="black")
                ax1.add_patch(rectangle)
            case 2:
                rectangle = plt.Rectangle((splt1[i], splt1[i+1]), h, h, fc='black',ec="black")
                ax2.add_patch(rectangle)
            case 3:
                rectangle = plt.Rectangle((splt1[i], splt1[i+1]), h, h, fc='black',ec="black")
                ax3.add_patch(rectangle)
        
    ax1.set_xlim([-1, 1])
    ax1.set_ylim([-1, 1])
    # ax1.grid()
    canvas1.draw()

    ax2.set_xlim([-1, 1])
    ax2.set_ylim([-1, 1])
    # ax2.grid()
    canvas2.draw()

    ax3.set_xlim([-1, 1])
    ax3.set_ylim([-1, 1])
    # ax3.grid()
    canvas3.draw()


# def clear_all():
#     for i in entery_list:
#         i.delete(0, 'end')

def block_res():
    [x.config(state='readonly') for x in res_entries]

def clear_res():
    for i in res_entries:
        i.config(text='')

def fill_res(values):
    for i in range(len(values)):
        res_entries[i].insert(0, values[i])

def show_matr_1():
    matrix_box.delete("1.0", END)
    matrix_box.insert("1.0", "2 1 1\n-2 0 -1\n2 1 2")

def show_matr_2():
    matrix_box.delete("1.0", END)
    matrix_box.insert("1.0", "0.7 -0.5 0\n0.5 0.7 0\n1 2 5")

def show_matr_3():
    matrix_box.delete("1.0", END)
    matrix_box.insert("1.0", "1 -1 1\n1 1 -1\n2 -1 0")

def show_matr_4():
    matrix_box.delete("1.0", END)
    matrix_box.insert("1.0", "4 -1 -2\n2 1 -2\n1 -1 1")

def show_matr_5():
    matrix_box.delete("1.0", END)
    matrix_box.insert("1.0", "0.5 2 0\n0 1 0\n0 0 6")

def show_matr_6():
    matrix_box.delete("1.0", END)
    matrix_box.insert("1.0", "0 -1 1\n-2 0 1\n0 1 0")

def show_matr_7():
    matrix_box.delete("1.0", END)
    matrix_box.insert("1.0", "5 -1 -1\n0 4 -1\n0 -1 4")

window = Tk()
window.title('ЦРМ в проективном пространстве')

menu = Menu(window)
window.config(menu=menu)
filemenu = Menu(menu, tearoff=0)
menu.add_cascade(label='Готовые матрицы', menu=filemenu)
filemenu.add_command(label='1(комплексные)', command=show_matr_2)
filemenu.add_command(label='2(разные)', command=show_matr_4)
filemenu.add_command(label='3(разные)', command=show_matr_5)
filemenu.add_command(label='4(кратные)', command=show_matr_6)
filemenu.add_command(label='5(кратные)', command=show_matr_7)


notebook = ttk.Notebook()
notebook.pack(expand=True, fill=BOTH)

matrix_frame = Frame(window)
matrix_frame.pack(expand=True, fill=BOTH)
notebook.add(matrix_frame, text='матрица')

matrix_label = Label(matrix_frame, text='Матрица')
matrix_label.pack()

matrix_box = Text(matrix_frame, width=10, height=3)
matrix_box.pack()    

h_label = Label(matrix_frame, text='Начальный диаметр ячеек')
h_label.pack()

h_entry = Entry(matrix_frame, width=10)
h_entry.pack()


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

res7_label = Label(frame211, text='Собственные числа матрицы: ', font=("Arial", 14), anchor='e')
res7_label.pack(expand=True, fill=BOTH)

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

res7_value = Entry(frame212, font=("Arial", 14))
res7_value.pack(expand=True, fill=BOTH)

iter_frame = Frame(frame2)
iter_frame.pack(expand=True, fill=BOTH)

iter_label = Label(iter_frame, text='Сколько итераций делать за один раз: ', font=("Arial", 14), anchor='e')
iter_label.pack(side=LEFT, expand=True, fill=BOTH)

spinbox_var = StringVar(value=1)
spinbox = ttk.Spinbox(iter_frame, from_=1.0, to=5.0, textvariable=spinbox_var)
spinbox.pack(side=LEFT, expand=True, fill=X)

start_btn = Button(frame2, text='Старт', command=true_start)
start_btn.pack(pady=10)

iter_btn = Button(frame2, text='Итерировать', command=iter)
iter_btn.pack(pady=10)

draw_btn = Button(frame2, text="Показать результат", command=draw_result)
draw_btn.pack(pady=10)




# def buttfun():
#     matrix_text = matrix_box.get(1.0, END)
#     print(matrix_text.split())

# butt = Button(matrix_frame, text='матрицв', command=buttfun)
# butt.pack()

# matrix_text = matrix_box.get(1.0, END)

fig_frame = Frame(frame2)
fig_frame.pack(expand=True, fill=X)

fig1 = Figure(figsize=(3,3))
ax1 = fig1.add_subplot(111)
canvas1 = FigureCanvasTkAgg(fig1, master = fig_frame)
canvas1.draw()
# toolbar1 = NavigationToolbar2Tk(canvas1, fig_frame, pack_toolbar=False)
# toolbar1.update()
canvas1.get_tk_widget().pack(side=LEFT, expand=True, fill=BOTH)
# toolbar.pack(expand=True, fill=X)

fig2 = Figure(figsize=(3,3))
ax2 = fig2.add_subplot(111)
canvas2 = FigureCanvasTkAgg(fig2, master = fig_frame)
canvas2.draw()
# toolbar1 = NavigationToolbar2Tk(canvas1, fig_frame, pack_toolbar=False)
# toolbar1.update()
canvas2.get_tk_widget().pack(side=LEFT, expand=True, fill=BOTH)

fig3 = Figure(figsize=(3,3))
ax3 = fig3.add_subplot(111)
canvas3 = FigureCanvasTkAgg(fig3, master = fig_frame)
canvas3.draw()
# toolbar1 = NavigationToolbar2Tk(canvas1, fig_frame, pack_toolbar=False)
# toolbar1.update()
canvas3.get_tk_widget().pack(side=LEFT, expand=True, fill=BOTH)

# entery_list = [x_entry,y_entry,x1_entry, y1_entry, x2_entry, y2_entry, paralpha_entry, parbeta_entry, park_entry, parb_entry, paromega_entry]
res_entries = [res1_value, res5_value, res3_value, res4_value, res6_value]

# fill_map()

window.mainloop()
