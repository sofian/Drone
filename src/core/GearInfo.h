#ifndef INC_GEARINFO
#define INC_GEARINFO

#include <qstring.h>
#include <qmap.h>
#include <vector>
#include "XMLHelper.h"
#include "AbstractPlug.h"
#include "frei0r.h"


class Gear;
class Schema;

/**
 * Pluginfo provide the meta information associated with each plug of the gear. PlugInfo are saved with GearInfo.
 *
**/ 
class PlugInfo
{
public:
	PlugInfo(){}
  PlugInfo(QString name, eInOut inOut, QString type, QString help="") :
	  _name(name),
		_inOut(inOut),
		_type(type),
		_help(help)
		{}

	QString name() const {return _name;}
	void name(QString v){_name=v;}

	eInOut inOut() const {return _inOut;}
	void inOut(eInOut v){_inOut=v;}

	QString type() const {return _type;}
	void type(QString v){_type=v;}

	QString help() const {return _help;}
	void help(QString v){_help=v;}

	bool save(QDomDocument doc, QDomElement parent)  const;
	bool load(QDomElement elem);
		
private:		
	QString _name;
	eInOut _inOut;
  QString _type;
  QString _help;
};

/**
 * GearInfo provide every information related to a Gear plugin and is actually used to instanciate
 * gears. GearInfo manage the plugin file and the metaInfo file.
 *
 * Each type of drone plugins inherit GearInfo. The createGearInstance(), metaFile() and bindPlugin() method must be implemented.
**/ 
class GearInfo
{
public:
  enum eGearPluginType {DRONE, FREI0R, META};

	GearInfo(eGearPluginType pluginType, QFileInfo pluginFile);
	virtual ~GearInfo(){}

  eGearPluginType pluginType() const {return _pluginType;}
	
  virtual bool save();
  bool load();	
  bool createDefaultMetaInfo();
	
	virtual QFileInfo metaFile()=0;//! mangling for the metaInfo filename from the pluginfile
	QString name(){return metaFile().baseName();}//! name of the gearInfo is taken from the metaFile name.
	virtual Gear* createGearInstance(Schema *schema, QString uniqueName)=0;

  bool setPlugInfo(const PlugInfo &plugInfo);
	PlugInfo getPlugInfo(QString name) {return _plugsInfo.value(name);}
	QMap<QString, PlugInfo> getAllPlugsInfo(){return _plugsInfo;}

protected:
  void syncPlugsInfo();
	bool addPlugInfo(const PlugInfo& pi);
	
  virtual bool loadMetaInfo();
	virtual bool bindPlugin()=0;//! bind to the plugin file

	static const QString XML_TAGNAME;

	eGearPluginType _pluginType;
	int _majorVersion;
  int _minorVersion;
  QString _author,_intro,_description;
  QMap<QString, PlugInfo> _plugsInfo;
  QStringList _classification;
  QFileInfo _pluginFile;
};

/**
 * GearInfo for drone gears.
**/
class GearInfoDrone : public GearInfo
{
public:
	GearInfoDrone(QFileInfo pluginFile);
	~GearInfoDrone();				
											
	Gear* createGearInstance(Schema *schema, QString uniqueName);

	QFileInfo metaFile()
	{
		return QFileInfo(_pluginFile.absolutePath() + "/" + _pluginFile.baseName().mid(3) + ".xml");
	}
					
protected:
	bool bindPlugin();

  Gear* (*_makeGear)(Schema *schema, QString uniqueName);	
	void* _handle;
};

/**
 * GearInfo for drone Frei0r gears.
**/
class GearInfoFrei0r : public GearInfo
{
public:
	GearInfoFrei0r(QFileInfo pluginFile);
	~GearInfoFrei0r();				
												  
	QFileInfo metaFile()
	{
		return QFileInfo(_pluginFile.absolutePath() +  "/" + _pluginFile.baseName() + ".xml");
	}
										
	Gear* createGearInstance(Schema *schema, QString uniqueName);
	bool bindPlugin();
					
protected:
	bool loadMetaInfo();

	void* _handle;
	f0r_plugin_info_t _pluginInfo;

};

/**
 * GearInfo for MetaGears.
**/
class GearInfoMeta : public GearInfo
{
public:
	GearInfoMeta(QFileInfo pluginFile);
	~GearInfoMeta();				

	QFileInfo metaFile()
	{
		return QFileInfo(_pluginFile.absolutePath() +  "/" + _pluginFile.baseName() + ".xml");
	}

	Gear* createGearInstance(Schema *schema, QString uniqueName);

protected:
	bool bindPlugin();

};

#endif