package drone.jmf.demos;

import ptolemy.actor.TypedCompositeActor;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.domains.sdf.kernel.SDFDirector;
import drone.jmf.actors.MovieReader;
import drone.jmf.actors.ImageDisplay;

//import ptolemy.kernel.util.Workspace;
//import ptolemy.domains.de.kernel.DEDirector;
//import ptolemy.domains.dt.kernel.DTDirector;
//import ptolemy.domains.ct.kernel.CTDirector;
import ptolemy.actor.Manager;
//import ptolemy.actor.lib.StringConst;
//import ptolemy.actor.lib.Writer;

//import java.awt.*;
//import java.awt.event.*;
//import java.awt.image.*;

public class JMFMovieDemo {

	public static void main(String[] args) throws NameDuplicationException, IllegalActionException {
		
			TypedCompositeActor top = new TypedCompositeActor();
			
			// Create the director.
			SDFDirector director = new SDFDirector(top, "director");
			String period = Double.toString(1.0 / 100.0);
			director.period.setExpression(period);
			director.synchronizeToRealTime.setExpression("true");
			//director.iterations.setExpression("100");
			Manager manager = new Manager("manager");
			top.setManager(manager);
			
			// Create two actors.
			MovieReader reader = new MovieReader(top, "reader");
			ImageDisplay writer = new ImageDisplay(top, "display");
			reader.fileOrURL.setExpression(args[0]);
			reader.isFrameBased.setExpression("true");
			top.connect(reader.output, writer.input);
			
			top.getManager().startRun();
	}
}
