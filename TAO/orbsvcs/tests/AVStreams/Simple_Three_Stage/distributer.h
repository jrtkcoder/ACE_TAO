/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams/Simple_Three_Stage
//
// = FILENAME
//    distributer.h
//
// = DESCRIPTION
//    Process to receive data from the sender and send it to the receiver
//
// = AUTHOR
//    Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
//
// ============================================================================

#ifndef DISTRIBUTER_H
#define DISTRIBUTER_H

#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"

class Distributer_Callback : public TAO_AV_Callback
{
  // = TITLE
  //    Defines a class for the distributer application callback
  //    for receiving data.
  //
  // = DESCRIPTION
  //    This class overides the methods of the TAO_AV_Callback so the 
  //    AVStreams can make upcalls to the application.
  
public:

  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     const ACE_Addr &peer_address);
  // Method that is called when there is data to be received from the sender

  int handle_destroy (void);
  // Called when the sender has finished reading the file and wants
  // to close down the connection.
  
  void flowname (const ACE_CString &flowname);
  ACE_CString flowname (void);
  // Accessor methods to set and get the flowname corresponding
  // to the callback
  

protected:
  ACE_CString flowname_;

};

class Distributer_StreamEndPoint : public TAO_Client_StreamEndPoint
{
  // = TITLE
  //    Defines the distributer aplication stream endpoint
  //
  // = DESCRIPTION
  //    This is the class that overrides the tao_server_endpoint to handle
  //    pre and post connect processing.
public:

  
  int set_protocol_object (const char *,
			   TAO_AV_Protocol_Object *object);
  // Store the reference to the protocol object corresponding
  // to the transport
  
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback);
  // Create the distributer application sender callback.
  

private:
  Distributer_Callback callback_;
  // Reference to the application callback.
};

class Distributer
{
  // = TITLE
  //    Defines the distributer application class.
  // 
  // = DESCRIPTION
  //    The distributer progarm that acts as intermediate receiver
  //    that receives data from the sender process.
public:
  Distributer (void);
  // Constructor

  ~Distributer (void);
  // Destructor.

  int init (int argc,
            char **argv,
            CORBA::Environment &);
  // Initialize data components.

  int set_sender_protocol_object  (TAO_AV_Protocol_Object *object);
  TAO_AV_Protocol_Object* get_sender_protocol_object (void);
  // Accessor methods to set/get the protocol object of the receiver/sender
  //  process

  int bind_to_mmdevice (AVStreams::MMDevice_ptr &mmdevice,
			const ACE_CString &mmdevice_name,
			CORBA::Environment &ACE_TRY_ENV);
  // Resolve the reference of the mmdevice from the naming service.

  TAO_StreamCtrl* get_receiver_streamctrl (void);
  // Get the stream control of the receiver

  void stream_created (void);
  // Called when stream created

  void stream_destroyed (void);
  // Called when stream destroyed

  int parse_args (int argc,
                  char **argv);

  int done (void);
  // Return the flag that suggests orb shutdown.

protected:
  TAO_Naming_Client my_naming_client_;
  // The Naming Service Client.

  TAO_AV_Endpoint_Reactive_Strategy_A 
  <Distributer_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> a_endpoint_strategy_;
  // The reactive strategy of the distributer receiver.

  TAO_MMDevice* distributer_mmdevice_;
  // The distributer receiver multimedia device
  
  AVStreams::MMDevice_var receiver_mmdevice_;
  // The receiver MMDevice.

  AVStreams::MMDevice_var sender_mmdevice_;
  // The sender MMDevice.

  TAO_AV_Protocol_Object *sender_protocol_object_;
  // The sender protocol object

  int count_;
  // Number of frames sent.
  
  ACE_CString sender_address_;
  // Address of the  distributer host machine or a multicast address 

  ACE_CString receiver_address_;
  // Address of the  distributer host machine or a multicast address 
  
  ACE_CString protocol_;
  // Selected protocol - default is UDP

  TAO_StreamCtrl* receiver_streamctrl_;
  // Video stream controller for the stream between receivers and distributer

  int stream_count_;
  // Number of active streams. When a stream is disconnected this 
  // count is decremented.

  int done_;
  // Flag to indicate orb shutdown

  ACE_CString host_sender_port_;
  //Distributer sender streamendpoint port

  ACE_CString host_receiver_port_;
  //Distributer receiver streamendpoint port
};

typedef ACE_Singleton<Distributer, ACE_Null_Mutex> DISTRIBUTER;

#endif /*DISTRIBUTER_H*/



