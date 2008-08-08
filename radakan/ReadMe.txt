To Build Radakan you will need to include the following librarys under lib:
jme.jar, lwjgl.jar, lwjgl_util.jar, and log4j.jar

You will also need to include in your execution arguments the location of the
.dll/.so files needed to use openGL and lwjgl. This is done by specifying
the java.exe argument -Djava.library.path="<<some absolute prefix>/lib/lwjgl/<os>/>"
(this is the VM argument in eclipse).

The Main application to run is Main.java.

Hope this gets you up and started. 