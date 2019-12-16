/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Universita' degli Studi di Napoli "Federico II"
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Pasquale Imputato <p.imputato@gmail.com>
 * Author: Stefano Avallone <stefano.avallone@unina.it>
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/traffic-control-module.h"
#include "ns3/flow-monitor-module.h"

// This simple example shows how to use TrafficControlHelper to install a 
// QueueDisc on a device.
//
// The default QueueDisc is a pfifo_fast with a capacity of 1000 packets (as in
// Linux). However, in this example, we install a RedQueueDisc with a capacity
// of 10000 packets.
//
// Network topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//
// The output will consist of all the traced changes in the length of the RED
// internal queue and in the length of the netdevice queue:
//
//    DevicePacketsInQueue 0 to 1
//    TcPacketsInQueue 7 to 8
//    TcPacketsInQueue 8 to 9
//    DevicePacketsInQueue 1 to 0
//    TcPacketsInQueue 9 to 8
//
// plus some statistics collected at the network layer (by the flow monitor)
// and the application layer. Finally, the number of packets dropped by the
// queuing discipline, the number of packets dropped by the netdevice and
// the number of packets requeued by the queuing discipline are reported.
//
// If the size of the DropTail queue of the netdevice were increased from 1
// to a large number (e.g. 1000), one would observe that the number of dropped
// packets goes to zero, but the latency grows in an uncontrolled manner. This
// is the so-called bufferbloat problem, and illustrates the importance of
// having a small device queue, so that the standing queues build in the traffic
// control layer where they can be managed by advanced queue discs rather than
// in the device layer.

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TrafficControlExample");

int
main (int argc, char *argv[])
{
  double simulationTime = 10; //seconds
  std::string socketTypeTCP = "ns3::TcpSocketFactory",socketTypeUDP = "ns3::UdpSocketFactory";
  NodeContainer nodes;
  nodes.Create (4);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  pointToPoint.SetQueue ("ns3::DropTailQueue", "MaxSize", StringValue ("10p"));

  NetDeviceContainer devices02,devices12,devices23;
  devices02 = pointToPoint.Install (nodes.Get(0),nodes.Get(2));
  devices12 = pointToPoint.Install (nodes.Get(1),nodes.Get(2));
  devices23 = pointToPoint.Install (nodes.Get(2),nodes.Get(3));

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces02 = address.Assign (devices02);
  address.SetBase ("10.1.2.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces12 = address.Assign (devices12);
  
  address.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces23 = address.Assign (devices23);
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  //TCP Flow
  uint16_t port = 7;
  Address localAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
  PacketSinkHelper packetSinkHelper (socketTypeTCP, localAddress);
  ApplicationContainer sinkApp = packetSinkHelper.Install (nodes.Get (3));

  sinkApp.Start (Seconds (0.0));
  sinkApp.Stop (Seconds (simulationTime + 0.1));

  OnOffHelper onoff (socketTypeTCP, Ipv4Address::GetAny ());
  onoff.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  onoff.SetAttribute ("DataRate", StringValue ("10Mbps")); //bit/s
  ApplicationContainer appsTCP;

  InetSocketAddress rmtTCP (interfaces23.GetAddress (1), port);
  AddressValue remoteAddressTCP (rmtTCP);
  onoff.SetAttribute ("Remote", remoteAddressTCP);
  appsTCP.Add (onoff.Install (nodes.Get (0)));
  appsTCP.Start (Seconds (1.0));
  appsTCP.Stop (Seconds (simulationTime + 0.1));


   //UDP Flow
  uint16_t portUDP = 9;
  Address localAddressUDP (InetSocketAddress (Ipv4Address::GetAny (), portUDP));
  PacketSinkHelper packetSinkHelperUDP (socketTypeUDP, localAddressUDP);
  ApplicationContainer sinkAppUDP = packetSinkHelperUDP.Install (nodes.Get (3));

  sinkAppUDP.Start (Seconds (0.0));
  sinkAppUDP.Stop (Seconds (simulationTime + 0.1));

  OnOffHelper onoffUDP (socketTypeUDP, Ipv4Address::GetAny ());
  onoffUDP.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoffUDP.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  onoffUDP.SetAttribute ("DataRate", StringValue ("10Mbps")); //bit/s
  ApplicationContainer apps;

  InetSocketAddress rmtUDP (interfaces23.GetAddress (1), portUDP);
  AddressValue remoteAddressUDP (rmtUDP);
  onoffUDP.SetAttribute ("Remote", remoteAddressUDP);
  apps.Add (onoffUDP.Install (nodes.Get (1)));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (simulationTime + 0.1));



  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  Simulator::Stop (Seconds (simulationTime + 5));
  Simulator::Run ();

  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
  std::cout << std::endl << "*** Flow monitor statistics ***" << std::endl;
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
      std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
      std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
      std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
      std::cout << "  lost Packets:   " << i->second.lostPackets << "\n";
      std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 9.0 / 1000 / 1000  << " Mbps\n";
    }

  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
