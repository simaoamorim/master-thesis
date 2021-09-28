# Real-time Ethernet networks: a practical aproach to cycle time influence in control applications

Copyright (c) 2020-2021 Simão Amorim <up201605618@edu.fe.up.pt>

# Abstract (quoted from the main document)
We live in an increasingly digital and computerised world where there is a constant need for interconnection between everything and everyone.
Ethernet networks quickly became the communication standard in office and home environments, but their adoption in the industrial environment has been much slower.
This reduced speed of adoption is mainly due to the non-deterministic communication that Ethernet network provides, which does not make it a viable option for automation and robotics systems that require predictable and deterministic communications.
Modern automation and robotic systems do not escape the accelerated need for constant interconnection and, therefore, it is necessary to adapt them, taking into account their real-time requirements.
There are several well-established real-time Ethernet network solutions on the market, but we find the same gap on all of them: the scarcity of educational and demonstration equipment.

This document aims at presenting a solution for this gap, describing a distributed control system developed with education in mind.
The presented system focuses on the variable that mostly contributes to the deterministic characteristic of real-time Ethernet networks: the network cycle time.
The main objective is to develop a conceptual distributed control system capable of producing experimental data that demonstrates the impact that the communication network's cycle time has on control applications.
The proposed system will base itself on the implementation of a slave device for the EtherCAT network, implemented on a Raspberry Pi platform, and on the description of a possible implementation of a master device for the same network.
The implementation of the master device will purposely not be specified in detail to encourage greater involvement from the users of this system in implementing their own educational or demonstration system.

The document will begin with a presentation of the context in which this work was developed, followed by the motivation that led to the beginning of the project and a presentation of the objectives that we propose to achieve.
The state-of-the-art will consist of a presentation of real-time Ethernet networks in a generic way and an in-depth presentation of the working principle and characteristics of the EtherCAT network.
The presentation of generic real-time Ethernet networks will describe the existing categories and different approaches to the problem of non-deterministic communication in Ethernet networks.
The presentation and description of the architecture of the proposed distributed control system will follow.
Next, an explanation on how the implementation of the slave device was planned and executed, both in terms of hardware and software.
Finally, experimental results will also be presented.
These prove that the developed concept is valid and fulfils the intended characteristics.

## Slave device software source-code
See the [src](/src) directory for the source code and instructions.

## Master thesis document
The LaTeX source-code for the final document is in the [document](/document) folder.

## Summary article
The LaTeX source-code for the summary article is in the [summary-article](/summary-article) folder.

## Presentation
The LaTeX source-code for the summary article is in the [presentation](/presentation) folder.
