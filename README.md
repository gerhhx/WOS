# Walk On Spheres
This is one of the earliest realisation versions I did of Walk On Spheres, a stochastic method for solving the Dirichlet problem for Poisson equation. 

## What Does The Program Do?
It solves the Dirichlet problem for Poisson equation using Walk On Spheres method in one specific point, chosen in the program manually.

## How Does The Method Work?
We want to find the solution for
$$\begin{cases}
\Delta u=0, \ x\in G\subset\mathbb{R}^n \\
u|_{\partial G}=f(x), \ x\in\partial G
\end{cases}
$$
in given point x. Let's set n=3 for simplicity.
From the chosen point it builds the smallest sphere of a radius $\rho$ which boundary reaches the boundary of the given domain (domain is a ball with the center in (0,0,0) and the radius of one here in program).
Then it draws random uniform value for radial and random uniform value for azimuthal angles $(\phi, \ \theta)$.
After that, it moves to the point $(\rho, \ \phi, \ \theta)$ in spheric coordinate system.
Then it repeats the same from ($\rho, \ \phi, \ \theta$), and gets another point, and does this until the point reaches the border in point ($x_1^*$), $~-$ and that's only one iteration of the algorithm.
It does this $N$ times from given point $x,$ and after that, it will have the list of $N$ points ${x_i^*}$ on the border. And the value of $u(x_i^*)$ is known because of Diriclet condition.
And the answer for given point $x$ is $u(x)=\sum_{i=1}^N u(x_i^*)$
