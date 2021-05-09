API
======================================================
Auto-generated documentation from Tristeon's source code/comments, combined with manually written samples and explanations.


Core
^^^^^^

The few most important core classes are placed right inside the src folder and are very commonly used or referenced across the engine. 

==================
TObject
==================
.. doxygenclass:: Tristeon::TObject
   :members:

==================
Collector
==================
.. doxygenclass:: Tristeon::Collector
   :members:

| **Usage**
| If a type of object wants to be collected, it is recommended to do so in its constructor and destructor:

.. code-block:: cpp

   class MyClass 
   {
   public:
      MyClass()
      {
         Collector<MyClass>::add(this);
      }

      ~MyClass()
      {
         Collector<MyClass>::remove(this);
      }

      void doThing()
      {
         //...
      }
   };

The collected objects are easily accessed like so:

.. code-block:: cpp

   //The objects can then be accessed through the all function:
   auto instances = Collector<MyClass>::all();

   for (auto* instance : instances) //and e.g. iterated over
   {
      instance->doThing();
   }

==================
InstanceCollector
==================
.. doxygenclass:: Tristeon::InstanceCollector
   :members:

| **Usage**
| InstanceCollector is used to find InstancedSerializables back in runtime by their instanceID. This can be used in for example serialization to reference objects like in this camera follow class:

.. code-block:: cpp

   class CameraFollow : public Behaviour, public IStart, public ILateUpdate
   {
   public:
      nlohmann::json serialize() override
      {
         json j = Behaviour::serialize();
         j["target"] = _target->instanceID(); //Store the target's instanceID in our json data
         return j;
      }

      void deserialize(nlohmann::json j) override
      {
         Behaviour::deserialize(j);
         _targetID = j.value("target", 0u); //Load the target's instanceID
         //InstanceCollector::find usually can't be used in deserialization time because objects might not have been created yet or their instanceID might not have been set yet
      }

      //Start is called when the scene and all of its objects have been fully created
      void start() override
      {
         //Now use the InstanceCollector to find the target back
         _target = InstanceCollector::find(_targetID);
      }

      //LateUpdate is called after all the other update calls, meaning that this Camera behaviour is more likely to get the right target position in case the target moves in update.
      void lateUpdate() override
      {
         //The target might not exist
         if (_target == nullptr)
               return;

         actor()->position = _target->position; //We can now use the target's actual reference for gameplay interactions
      }
   private:
      unsigned int _targetID = 0;
      Actor* _target = nullptr;
   }

==================
Window
==================
.. doxygenclass:: Tristeon::Window
   :members:

==================
Engine
==================
.. doxygenclass:: Tristeon::Engine
   :members:

==================
Settings
==================
.. doxygenclass:: Tristeon::Settings
   :members:



Animations
^^^^^^^^^^^
==================
AnimationClip
==================
.. doxygenclass:: Tristeon::AnimationClip
   :members:



AssetManagement
^^^^^^^^^^^^^^^^^^
==================
Resources
==================
.. doxygenclass:: Tristeon::Resources
   :members:

==================
AssetDatabase
==================
.. doxygenclass:: Tristeon::AssetDatabase
   :members:

==================
Domain
==================
.. doxygenclass:: Tristeon::Domain
   :members:

==================
MetaFile
==================
.. doxygenclass:: Tristeon::MetaFile
   :members:



Audio
^^^^^^^^^^^
==================
Audio (class)
==================
.. doxygenclass:: Tristeon::Audio
   :members:

==================
AudioClip
==================
.. doxygenclass:: Tristeon::AudioClip
   :members:

==================
AudioListener
==================
.. doxygenclass:: Tristeon::AudioListener
   :members:

==================
AudioSource
==================
.. doxygenclass:: Tristeon::AudioSource
   :members:



Callbacks
^^^^^^^^^^^
==================
ICallback
==================
.. doxygenclass:: Tristeon::ICallback
   :members:

==================
IInit
==================
.. doxygenclass:: Tristeon::IInit
   :members:

==================
IEarlyUpdate
==================
.. doxygenclass:: Tristeon::IEarlyUpdate
   :members:

==================
IUpdate
==================
.. doxygenclass:: Tristeon::IUpdate
   :members:

==================
ILateUpdate
==================
.. doxygenclass:: Tristeon::ILateUpdate
   :members:

==================
IFixedUpdate
==================
.. doxygenclass:: Tristeon::IFixedUpdate
   :members:

==================
IPreDestroy
==================
.. doxygenclass:: Tristeon::IPreDestroy
   :members:

==================
IDrawGizmos
==================
.. doxygenclass:: Tristeon::IDrawGizmos
   :members:

==================
ISceneLoaded
==================
.. doxygenclass:: Tristeon::ISceneLoaded
   :members:



Editor
^^^^^^^^^^^
==================
Placeholder
==================



Environment
^^^^^^^^^^^
==================
Placeholder 2
==================



Game
^^^^^^^^^^^
==================
Placeholder 3
==================



Input
^^^^^^^^^^^
==================
Placeholder 4
==================



Math
^^^^^^^^^^^
==================
Placeholder 5
==================



Physics
^^^^^^^^^^^
==================
Placeholder 6
==================



Rendering
^^^^^^^^^^^
==================
Placeholder 7
==================



Scenes
^^^^^^^^^^^
==================
Placeholder 8
==================



Serialization
^^^^^^^^^^^^^^
==================
Placeholder 9
==================



Standard
^^^^^^^^^^^
==================
Placeholder 10
==================



Utils
^^^^^^^^^^^
==================
Placeholder 11
==================