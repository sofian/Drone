#ifndef SCHEMA_INCLUDED
#define SCHEMA_INCLUDED

#include <QtXml>
#include <QObject>
#include <QMap>
#include "Connection.h"

class MetaGear;
class Gear;
class AbstractPlug;
class Engine; 


class Schema : public QObject
{
public:
  static const QString XML_TAGNAME;

  class ScheduledConnection
  {
  public:
    ScheduledConnection(const AbstractPlug &a, const AbstractPlug &b) : _a(&a), _b(&b) {}    
    const AbstractPlug *_a;
    const AbstractPlug *_b;    
  };

  Schema(MetaGear &parentMetaGear);
  virtual ~Schema();

	/////////////////////////////////////////////////////////////////////////////
  //serialization 
	/////////////////////////////////////////////////////////////////////////////
  bool save(QDomDocument& doc, QDomElement &parent, bool onlySelected=false);
	bool load(QDomElement& doc, bool pasting=false);
  
	///////////////////////////////////////////////////////////////////////////// 
	//Gears operations
	/////////////////////////////////////////////////////////////////////////////
  //! Returns a list of unordered gears, but not expanded. Metagears are left as is.
  QList<Gear*> getGears(){return _gears.values();}  
  //! Returns a list of expanded gears, as GetGears, but where metagears are replaced by their internal schema.
  QList<Gear*> getDeepGears() const;
  //! Returns a list of ordered gears
  QList<Gear*> getDeepOrderedReadyGears();
  void renameGear(Gear &gear, QString newName);
  Gear* addGear(QString type);  
  bool removeDeepGear(const Gear& gear);
  Gear* getGearByName(QString name) const;
  void clear();

	///////////////////////////////////////////////////////////////////////////// 
	//Connections
	/////////////////////////////////////////////////////////////////////////////
  void getAllConnections(QList<Connection*> &connections);
  bool connect(AbstractPlug &plugA, AbstractPlug &plugB);
  bool connect(Connection &connection);
  void disconnect(AbstractPlug &plugA, AbstractPlug &plugB);
  void disconnectAll(AbstractPlug &plugA);

	///////////////////////////////////////////////////////////////////////////// 
	//Schema/Sub-Schema operations
	/////////////////////////////////////////////////////////////////////////////  
  QList<Schema*> getSubSchemas();
  Schema *getParentSchema();
    
	///////////////////////////////////////////////////////////////////////////// 
	//Thread-safety
	/////////////////////////////////////////////////////////////////////////////	
	void lock(){_locked=true;}
  void unlock();
  
	//TODO:wtf?	
  void unSynch(){_needSynch=true;}  
			  	
signals:
	void gearAdded(const Schema &schema, const Gear &gear);
	void gearRemoved(const Schema &schema, const Gear &gear);
	void gearRenamed(const Schema &schema, const Gear &gear);
	void connectionCreated(const Schema &schema, Connection connection);
	void connectionDeleted(const Schema &schema, Connection connection);
					
private:
  QString mangleUniqueGearName(QString originalName);

	void onGearAdded(const Gear &gear);
	void onGearRemoved(const Gear &gear);

  bool needSynch();
  void synch();
  bool _needSynch;

	QMap<QString, Gear*> _gears;
  QList<Gear*> _lastDeepOrderedReadyGears;
  MetaGear *_parentMetaGear;
  
	//thread safety
  bool _locked;
  std::vector<ScheduledConnection> _scheduledConnections;
  std::vector<ScheduledConnection> _scheduledDisconnections;
  std::vector<Gear*> _scheduledDeletes;
};

#endif
