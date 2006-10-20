#ifndef METAGEAR_INCLUDED
#define METAGEAR_INCLUDED

#include "Gear.h"
#include "Schema.h"
#include "ISchemaEventListener.h"

#include <map>

class ControlPanel;

class MetaGear : public Gear, public ISchemaEventListener
{
public:  
  MetaGear(Schema *parentSchema, std::string name, std::string uniqueName);
  virtual ~MetaGear();
  virtual Schema* getInternalSchema(){return _schema;}
  
  GearKind kind() const {return METAGEAR;}

  static const std::string TYPE;
  static const std::string EXTENSION;
  
  void save(std::string filename);
  bool load(std::string filename);
  
  void createPlugs();

  std::string fullPath(){return _fullPath;}
  
  void onGearAdded(Schema *schema, Gear *gear);
  void onGearRemoved(Schema *schema, Gear *gear);

  void associateControlPanel(ControlPanel *controlPanel) {_associatedControlPanel = controlPanel;}
  
protected:
  
  void internalSave(QDomDocument &doc, QDomElement &parent);
  void internalLoad(QDomElement &parent);
  
  GearGui* createGearGui(QCanvas *canvas);
  	
  Schema *_schema;
  
  std::string _metaGearName;
  std::string _fullPath;

  ControlPanel *_associatedControlPanel;

  static const QColor METAGEAR_COLOR;
	
  std::map<AbstractPlug*, AbstractPlug*> _plugMapping;//!mapping between our exposed cloned plug and the real internal plug
};

#endif
