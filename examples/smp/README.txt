---------------------------------------------
 Copyright KAPSARC. Open Source MIT License.
---------------------------------------------

The 'smp' example builds a simple, but highly parameterizable, implementation
of the multi-dimensional Spatial Model of Politics.

There are three components:
* libsmp: this is the library which contains all the working parts of the model.
* smpc: this is a command line application, suitable for use either directly
  from the command line or as part of the backend of a web application
* spmg: this is a stand-alone GUI application. 


"smpc" accepts a "--help" argument to display the help menu: 

smpApp version 0.1
Usage: specify one or more of these options
--help            print this message
--euSMP           exp. util. of spatial model of politics
--csv <f>         read a scenario from CSV
--seed <n>        set a 64bit seed
                  0 means truly random
                  default: 00000000000829961074
The --euSMP option generates a random multi-dimensional dataset and simulates
bargaining until the actor positions stabilize. They are considered stable
when the RMS change is 1/100-th of the RMS change on the first turn.
As the simulation generates a great deal of text output, it is recommended that
one pipe the output to a text file with a command such as the following:
   smpApp --seed 31416 --euSMP  > tmp.txt

The "--csv" option reads in the named file and simulates bargaining until the actor positions stabilize.
An example of the expected format is in kmodel/doc/dummyData_3Dim.csv
Again, it is recommended that the output be piped into a text file.

The output consists of a great deal of detailed information as the bargaining progresses.
At the end, after the phrase "Completed model run", one can find the positions of the actors over the course of the simulation.
In the following excerpt,there are two actors (number 00 and 01) with positions in a three dimensional policy space
(dimensions 00, 01, and 02). Each coordinate is on a [0,100 scale]. The first column is the initial position of the
actors (randomly generated, in this case). Thus, the position of SActor-00 at time 0 is [5.330, 78.656, 77.921].
After one step of the simulation, SActor-00 moves to [87.131, 34.184, 42.949] at time 1.


History of actor positions over time
SActor-00 , Dim-00  ,   5.330  ,  87.131  ,  72.061  ,  72.061  ,  79.614  ,  79.614  ,  72.053  ,  72.053  ,  72.053  , 
SActor-00 , Dim-01  ,  78.656  ,  34.184  ,  34.904  ,  34.904  ,  36.190  ,  36.190  ,  56.741  ,  56.741  ,  56.741  , 
SActor-00 , Dim-02  ,  77.921  ,  42.949  ,  70.253  ,  70.253  ,  71.085  ,  71.085  ,  74.612  ,  74.612  ,  74.612  , 
SActor-01 , Dim-00  ,  74.961  ,  67.822  ,  71.127  ,  71.127  ,  71.127  ,  71.129  ,  72.052  ,  72.052  ,  72.052  , 
SActor-01 , Dim-01  ,  60.367  ,  60.340  ,  57.958  ,  57.958  ,  57.958  ,  57.958  ,  56.741  ,  56.741  ,  56.741  , 
SActor-01 , Dim-02  ,  81.793  ,  80.704  ,  74.989  ,  74.989  ,  74.989  ,  74.984  ,  74.612  ,  74.612  ,  74.612  , 

Below the history of positions comes an estimate of the likelihood each actor would be the favored option,
at each step of the simulation. In the excerpt below, SActor-01 has 32% probability in the initial state,
but only 18% in the final state. By contrast, SActor-06 rises from 8.6% to 19% probability.

SActor-00 , prob  , 0.0488  , 0.1808  , 0.1948  , 0.1770  , 0.1560  , 0.1355  , 0.1696  , 0.1834  , 0.1745  , 
SActor-01 , prob  , 0.3189  , 0.1814  , 0.1731  , 0.1671  , 0.1987  , 0.1845  , 0.1804  , 0.1936  , 0.1810  , 
SActor-02 , prob  , 0.1560  , 0.1364  , 0.1348  , 0.1013  , 0.0837  , 0.1319  , 0.1365  , 0.1269  , 0.1240  , 
SActor-03 , prob  , 0.1469  , 0.1149  , 0.1099  , 0.1057  , 0.1273  , 0.1189  , 0.1235  , 0.1101  , 0.1172  , 
SActor-04 , prob  , 0.1618  , 0.1232  , 0.1106  , 0.1105  , 0.1088  , 0.1158  , 0.0932  , 0.0886  , 0.0938  , 
SActor-05 , prob  , 0.0821  , 0.0899  , 0.0721  , 0.1357  , 0.1560  , 0.1329  , 0.1269  , 0.1178  , 0.1192  , 
SActor-06 , prob  , 0.0855  , 0.1734  , 0.2047  , 0.2027  , 0.1696  , 0.1803  , 0.1700  , 0.1796  , 0.1902  , 







---------------------------------------------
BUILD NOTES

In Visual Studio 2013 Express, you have to reset the Project -> Properties -> Linker -> 
System -> SubSystem properly in order to suppress the console;
see the attached screenshot "2015-01-30 setting console properties"


The cmake-gui tool builds a project file for a 
'Console' application, not a 'Windows' application.
Thus, it creates a black terminal window whenever it runs.
To fix this in MS Studio 2008, you can select the executable target,
right click to Properties->Linker->System->SubSystem, and
change it from Console to Application.

In MS Studio 2010 Professional, the process of suppressing the
console window for a project has two steps:
  Properties -> Linker -> System -> SubSystem: Windows(/SUBSYSTEM:WINDOWS)
  Properties -> Linker -> Advanced -> Entry Point: mainCRTStartup
or in the project build linker options set
  /SUBSYSTEM:windows
  /ENTRY:mainCRTStartup

See this posting for more details:
http://stackoverflow.com/questions/2139637/hide-console-of-windows-application/6882500#6882500
--------------------------------------------
Another nagging problem has been the inability to attach an icon to a program.
I finally found ways to do it in MS Studio 2010 Professional.

The procedure is (roughly) to open the View -> Resource View of the project
(similar to the Project View),
right-click on the project and follow Add -> Resource -> Icon.
I then pointed it to the existing ico files in the top-level directory, and a little more
fiddling got it attached. Note that the foo.rc files need to be in 'build':
you need to re-build them there, so these are only for reference.
---------------------------------------------
 Copyright KAPSARC. Open Source MIT License.
---------------------------------------------

