package drone.core;

import java.awt.Component;

import javax.swing.Icon;
import javax.swing.JFrame;

import ptolemy.actor.gui.Tableau;
import ptolemy.vergil.basic.BasicGraphFrame;

import net.infonode.docking.View;

@SuppressWarnings("serial")
public class TableauView extends View {

	public TableauView(Icon icon, Tableau tableau) {
		super(_getViewTitle(tableau), icon, _createViewComponent(tableau));
		// TODO Auto-generated constructor stub
		_tableau = tableau;
	}
	
	private static String _getViewTitle(Tableau tableau) {
		String viewTitle = tableau.getTitle();
		int slashIndex = viewTitle.lastIndexOf('/');
		if (slashIndex >= 0) {
			viewTitle = viewTitle.substring(slashIndex+1);
		}
		return viewTitle;
	}
	
	private static Component _createViewComponent(Tableau tableau) {
		JFrame frame = tableau.getFrame();
		if (frame instanceof BasicGraphFrame)
			return ((BasicGraphFrame)frame).getRightComponent();
		else
			return frame.getComponent(0);
	}
	
	public Tableau getTableau() { return _tableau; }
	
	private Tableau _tableau;

}
