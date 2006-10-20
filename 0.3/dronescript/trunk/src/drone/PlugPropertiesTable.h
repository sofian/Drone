/*
 *  PlugPropertiesTable.h
 *  drone
 *
 *  Created by foogy on 20/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <qtable.h>


class Gear;
class GearGui;
class AbstractPlug;

class PlugPropertiesTable : public QTable
{
	Q_OBJECT
public:
	PlugPropertiesTable(QWidget *parent);
  
	void refresh(Gear *gear);    
    

public slots:
	void slotGearSelected(GearGui *gearGui);

protected:
  void insertPlug(AbstractPlug *plug, int row);
	
private:
	Gear *_gear;
	
};
