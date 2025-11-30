#set document(title: [
  Homework 2 for computer scientists \ Report
])


#set page(
  paper: "a4",
  numbering: "1",
  columns: 2,
)

#set par(justify: true)
#set text(
  // font: "Droid Sans",
  size: 12pt,
)

#show title: set text(size: 17pt)
#show title: set align(center)
#show title: set block(below: 1.2em)

#place(
  top + center,
  float: true,
  scope: "parent",
  clearance: 2em,
)[
  #title()

  #grid(
    columns: (1fr),
    align(center)[
      Denys Andrukhovskyi, 3BIN \
      #link("mailto:andrukhovskyi1@uniba.sk")[#text(blue)[andrukhovskyi1\@uniba.sk]]
    ]
  )

]


= C++ Implementation
We use C++ to implement the algorithm. 
Code is divided into several files:
- `tree.h` and `tree.cpp` contain definitions of classes `tree` (phylogenetic rooted binary tree), `node` (main part of the `tree` class), `enum base` and some simple algorithms like getting postorder traversal of a tree;
- `alignment.h` and `alignment.cpp` contain definitions, that are used to define an alignment of multiple sequences; 
- `felsenstein.h` and `felsenstein.cpp` contain main algorithm, as well as workarounds for task `c)` and `d)`;
- `test.cpp` contain `main` function and functions for testing each substask.

== Phylogenetic tree
=== Class `tree`
We store rooted binary phylogenetic tree by using pointers paradigm: tree has a root, and we have pointer from each node to both children. 
Instance of class `tree` has two member fields: `root`, which is a pointer to a root (of class `node`), and `leaves`, which is a `vector` of pointers corresponding to all leaves of a tree.

In tree class we define one important static function, which calculates probability according to Jukes-Cantor substition model.

There is one unpleasant method: `set_leaves_bases`. 
We call this method every time we select new column as our alignment. 
There is a lot of overhead here, we can probably refactor this function into something better. 
For reasoning see part #link(<eff>)[#text(blue)[Effectivness]].

=== Class `node`
Instances of class `node` contain all information we need, each Instance has members for: 
- pointers to left and right child;
- distance from it to each children;
- name (of a taxon);
- which base has this node (makes sense only for leaves);
- precalculated transtion matrix for JC69 substition model for both children;
- internal index.

=== Effectivness <eff>
We are aware that probably if we'd use some other way to store the tree (e.g. storing tree as one array or in some different way use `std::vector<>` instead of pointers, so we save on cache allocations),
we would achieve a much better performance.
For example, method `set_leaves_bases` probably can be refactored, but refactoring probably would ruin the separation of code into methods, and we will get very effective spaghetti code: 
passing bases without changing the vertices means we need to pass alignment each time directly to the `felsenstein` function, where we need to efffectively complete the task of identifying where each leaf leaves, to "set" the base to start our DP algorithm.
That's probably doable, but not so pleasant.
So we use this "ineffective" implementetion, and that's for 2 reasons:
+ It's easier to read and change the code .
+ By passing `-O2` parameter into GCC compiler, we effectively get the speed which is satisfactory for our purposes ($tilde 2$  minutes vs $tilde 20 op(minus) 40$ minutes to run all tasks `a,b,c,d`). Purely magic. #emoji.wand.

== Felsenstein's algorithm
Felsenstein's algorithm is then implemented is `felsenstein.h` and `felsenstein.cpp`.
We will highlight both C++ technicalities and algorithm details. 

We start with setting bases for vertices.
This operation in it's nonoptimized version can take a lot of time

= Answers to questions
== Task a)

== Task b)
We calculated probability for $alpha = 1; alpha = 0.2$.
- Value for $alpha = 1$   is $0.0544541$.
- Value for $alpha = 0.2$ is $0.18337$.

== Task c)
We calculated maximum value for logarithm of probability and corresponding $alpha$.
- $log(P) = -3.3887 dot 10^6$,
- $alpha = 1.0$

== Task d)
Values of $alpha$ for the first ten windows can be seen on the @t1.

#set table(
  fill: (x, y) =>
    if x == 0 or y == 0 {
      gray.lighten(40%)
    },
  align: right,
)
#show figure: set block(breakable: true) 
#figure(
  table( 
  columns: (auto, auto, auto),
  inset: 10pt,
  align: horizon,
  table.header(
    [*Window*], [*Logarithm of \ probability*], [*Speed of \ evolution* $alpha$],
  ),
  [1-100],  [$-379.05$],[$1$],
  [101-200],[$-425.57$],[$1.2$],
  [201-300],[$-305.689$],[$1.1$],
  [301-400],[$-279.439$],[$2$],
  [401-500],[$-249.284$],[$1.6$],
  [501-600],[$-225.725$],[$1$],
  [601-700],[$-384.101$],[$1.5$],
  [701-800],[$-404.951$],[$1.3$],
  [801-900],[$-264.273$],[$2$],
  [901-1000],[$-447.686$],[$1.1$],
),
caption: "First ten windows"
) <t1>

== Task e)

#figure(
image("combined_distribution.png"),
caption: "Both distribution histograms"
)

#figure(
image("combined_cumulative_distribution.png"),
caption: "Both cumulative distribution functions"
)