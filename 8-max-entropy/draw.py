import os
import time
import numpy as np
from math import log

from matplotlib.figure import Figure
from matplotlib import colormaps
from matplotlib.colors import Normalize
from tkinter import *
from tkinter import ttk
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)

def clear_all():
    for i in entery_list:
        i.delete(0, 'end')

def fill_map():
    clear_all()
    x_entry.insert(0, 'x**2 - y**2 + a')
    y_entry.insert(0, '2*x*y+b')
    paralpha_entry.insert(0, '0.2')
    parbeta_entry.insert(0, '-0.2')
    x1_entry.insert(0,'-2')
    y1_entry.insert(0, '-2')
    x2_entry.insert(0, '2')
    y2_entry.insert(0, '2')
    h_entry.insert(0, '0.5')

def iterate_from_start():
    iterations = int(iter_entry.get())
    os.system('g++ main.cpp')
    mainiteration(iterations)

def mainiteration(iterations):
    global start
    x0 = eval(x1_entry.get()) #s1[0]
    y0 = eval(y1_entry.get()) #s1[1]
    x1 = eval(x2_entry.get()) #s2[0]
    y1 = eval(y2_entry.get()) #s2[1]
    somelist1 = [x0,y0]
    somelist2 = [x1,y1]
    # somelist1, somelist2 = (txtobl.text()).split("x")
    # somelist1 = eval(str(somelist1))
    # somelist2 = eval(str(somelist2))
    x0 = somelist1[0]
    y0 = somelist2[1]
    L = somelist2[0] - somelist1[0]
    H = somelist2[1] - somelist1[1]
    print(x0,y0,L,H)
    # somelist1, somelist2 = (listparam.text()).split(",")
    # a = str(somelist1).split("=")[1]
    # b = str(somelist2).split("=")[1]
    a = paralpha_entry.get()
    b = parbeta_entry.get()
    print(a,b)
    # iterations = globiterc1.text()
    
    #if iternum == int(globiterc1.text()):
        #res_neout1 = f"{matr}\nСобственные значения: {eig(matr)[0][0]} {eig(matr)[0][1]} {eig(matr)[0][2]}\n"
    start = time.time()
    os.system(f"./a.out {x0} {y0} {L} {H} {a} {b} {iterations}") # {a32} {a33} {iternum}")

def mymainpainter():
    global start
    ax.clear() 
    with open('res.osip') as f:
        lines = f.readlines()

    # print(lines[-1])
    # print(lines[-2])
    # toprintcomsnum = lines[-1].rstrip()
    # toprintcellamount = lines[-2].split(" ")[1]
    #res_neout1 = f"{res_neout1}Время подсчета { iternum } итераций: {restime}\nРазмер ячейки: {d}\n"
    # lines = lines[:-2]
    ax.clear()
    x=[]
    y=[]
    vals=[]
    _lambda=float(lines[0])
    d = float(lines[1])
    entropy = float(lines[-2])
    cells = float(lines[-1])
    res6_value.config(text=str(d))
    res7_value.config(text=str(log(_lambda)))
    res8_value.config(text=str(entropy))
    res1_value.config(text=str(cells))
    for i in range(2,len(lines)-2):
        x_t1, y_t1, val = lines[i].split()
        x.append(float(x_t1))
        y.append(float(y_t1))
        vals.append(float(val))

    cmap = colormaps['magma']
    norm = Normalize(vmin=min(vals), vmax=max(vals))
    colors = cmap(norm(vals))

    ax.bar3d(x, y, np.zeros_like(x), d, d, vals, shade=True, color=colors)
    canvas.draw()
    #plt.show()
    try:
        restime = time.time() - start
    except AttributeError:
        restime = 0
        iternum = 0
    # print(restime)
    res4_value.config(text=str(restime))



window = Tk()
window.title('Мера максимальной энтропии')

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

x_label = Label(frame111_left, text='f(x) = ', anchor='e')
x_label.pack(expand=True, fill=BOTH)

y_label = Label(frame111_left, text='f(y) = ', anchor='e')
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

paralpha_label = Label(frame112_left, text='a = ', anchor='e')
paralpha_label.pack(expand=True, fill=BOTH)

parbeta_label = Label(frame112_left, text='b = ', anchor='e')
parbeta_label.pack(expand=True, fill=BOTH)

h_label = Label(frame112_left, text='Начальный размер ячейки: ', anchor='e')
h_label.pack(expand=True, fill=BOTH)

iter_label = Label(frame112_left, text='Кол-во итераций: ', anchor='e')
iter_label.pack(expand=True, fill=BOTH)

frame112_right = Frame(frame112)
frame112_right.pack(side=LEFT, expand=True, fill=BOTH)

paralpha_entry = Entry(frame112_right)
paralpha_entry.pack(expand=True, fill=X)

parbeta_entry = Entry(frame112_right)
parbeta_entry.pack(expand=True, fill=X)

h_entry = Entry(frame112_right)
h_entry.pack(expand=True, fill=X)

iter_entry = Entry(frame112_right)
iter_entry.pack(expand=True, fill=X)

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

res1_label = Label(frame211, text="Количество ячеек в области: ", font=("Arial", 14), anchor='e')
res1_label.pack(expand=True, fill=BOTH)

# res3_label = Label(frame211, text="Время работы итерации: ", font=("Arial", 14), anchor='e')
# res3_label.pack(expand=True, fill=BOTH)

res4_label = Label(frame211, text="Общее ремя работы: ", font=("Arial", 14), anchor='e')
res4_label.pack(expand=True, fill=BOTH)

res6_label = Label(frame211, text='Диаметр ячейки: ', font=("Arial", 14), anchor='e')
res6_label.pack(expand=True, fill=BOTH)

res7_label = Label(frame211, text='ln(lambda): ', font=("Arial", 14), anchor='e')
res7_label.pack(expand=True, fill=BOTH)

res8_label = Label(frame211, text='Энтропия: ', font=("Arial", 14), anchor='e')
res8_label.pack(expand=True, fill=BOTH)

# delete_btn = Button(frame211, text="Очистить результаты")
#                     # , command = remove_kebab)
# delete_btn.pack()

frame212 = Frame(frame21)
frame212.pack(side=LEFT, expand=True, fill=BOTH)

res1_value = Label(frame212, text='', anchor='w', font=("Arial", 14))
res1_value.pack(expand=True, fill=BOTH)

# res3_value = Label(frame212, text='', anchor='w', font=("Arial", 14))
# res3_value.pack(expand=True, fill=BOTH)

res4_value = Label(frame212, text='', anchor='w', font=("Arial", 14))
res4_value.pack(expand=True, fill=BOTH)

res6_value = Label(frame212, text='', anchor='w', font=("Arial", 14))
res6_value.pack(expand=True, fill=BOTH)

res7_value = Label(frame212, text='', anchor='w', font=("Arial", 14))
res7_value.pack(expand=True, fill=BOTH)

res8_value = Label(frame212, text='', anchor='w', font=("Arial", 14))
res8_value.pack(expand=True, fill=BOTH)

# iter_frame = Frame(frame2)
# iter_frame.pack(expand=True, fill=BOTH)

# iter_label = Label(iter_frame, text='Сколько итераций делать за один раз: ', font=("Arial", 14), anchor='e')
# iter_label.pack(side=LEFT, expand=True, fill=BOTH)

# spinbox_var = StringVar(value=1)
# spinbox = ttk.Spinbox(iter_frame, from_=1.0, to=5.0, textvariable=spinbox_var)
# spinbox.pack(side=LEFT, expand=True, fill=X)

start_btn = Button(frame2, text='Выполнить', command=iterate_from_start)
start_btn.pack(pady=10)

iter_btn = Button(frame2, text='Нарисовать', command=mymainpainter)
iter_btn.pack(pady=10)

# draw_btn = Button(frame2, text="Показать результат")
# draw_btn.pack(pady=10)

fig = Figure()
ax = fig.add_subplot(111, projection='3d')
canvas = FigureCanvasTkAgg(fig, master = frame2)
canvas.draw()
toolbar = NavigationToolbar2Tk(canvas, frame2, pack_toolbar=False)
toolbar.update()
canvas.get_tk_widget().pack(expand=True, fill=BOTH)
toolbar.pack(expand=True, fill=X)

par_list = [paralpha_entry, parbeta_entry]
entery_list = [x_entry,y_entry,x1_entry, y1_entry, x2_entry, y2_entry] + par_list
res_entries = [res1_value, res4_value, res6_value]

fill_map()
window.mainloop()
