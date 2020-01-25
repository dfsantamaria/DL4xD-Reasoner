#DL4xD-Reasoner

A Reasoner for the Description Logic DL4xD based on the set-theoretic fragment 4LQS^R.

Copyright 2015 Domenico Cantone, Marianna Nicolosi Asmundo, Daniele Francesco Santamaria

This is the project page of DL4xD-Reasoner, a KE-tableau-based implementation of a reasoner for a decidable fragment of stratified set theory expressing the description logic DL4xD. Our solution solves the main TBox and ABox reasoning problems for DL4xD. In particular, it solves the consistency problem for DL4xD-knowledge bases represented in set-theoretic terms, and a generalization of the Conjunctive
Query Answering problem in which conjunctive queries with variables of three sorts are admitted, called Higher-Order Conjunctive Query Answering. The reasoner also solves the classification problem for DL4xD-knowledge bases. The reasoner, implemented in C++, supports DL4xD-knowledge bases serialized in the OWL/XML format for OWL 2 ontologies and admitting rules expressed in SWRL (Semantic Web Rule Language).

Publications of articles or papers refering to the DL4xD-Reasoner are listed below.

- Domenico Cantone, Marianna Nicolosi-Asmundo, and Daniele Francesco Santamaria. An optimized KE-tableau-based system for reasoning in the description logic DL4xD. In Benzmuller, C., Ricca, F., Parent, X., Roman, D. (Eds.). Rules and Reasoning Second International Joint Conference, RuleML+RR 2018, Luxembourg, Luxembourg, September 18–21, 2018. Lecture Notes in Computer Science 11092, Springer 2018, ISBN 978-3-319-99905-0.

- Domenico Cantone, Marianna Nicolosi-Asmundo, and Daniele Francesco Santamaria. A set-based reasoner for the description logic DL4xD. Proceedings of SETS 2018, Southampton, United Kingdom, 5 June 2018. CEUR Workshop Proceedings, ISSN 1613-0073, Vol. 2199.

- Domenico Cantone, Marianna Nicolosi-Asmundo, and Daniele Francesco Santamaria. A C++ reasoner for the description logic DL4xD. Proceedings of CILC 2017, 26-29 September 2017, Naples, Italy. CEUR Workshop Proceedings, ISSN 1613-0073, Vol. 1949, pp. 276-280.

- Domenico Cantone, Marianna Nicolosi-Asmundo, and Daniele Francesco Santamaria. A set-theoretic approach to ABox reasoning services. In Costantini S., Franconi E., Van Woensel W., Kontchakov R., Sadri F., Roman D. (eds) Rules and Reasoning. RuleML+RR 2017. Lecture Notes in Computer Science, vol 10364, pp. 87-102. Springer, Cham.

- Domenico Cantone, Marianna Nicolosi-Asmundo, and Daniele Francesco Santamaria. Conjunctive Query Answering via Fragment of Set Theory. Proc. of the 17th Italian Conference on Theoretical Computer Science, ICTCS 2016, Lecce, Italy, September 7-9, 2016, CEUR Workshop Proceedings, ISSN 1613-0073, Vol. 1720, pp. 23-35.

- Domenico Cantone, Cristiano Longo, Marianna Nicolosi-Asmundo, and Daniele Francesco Santamaria. Web ontology representation and reasoning via fragments of set theory. In: Cate B., Mileo A. (eds) Web Reasoning and Rule Systems. RR 2015. Lecture Notes in Computer Science, vol 9209. Springer, Cham (2015).

- Daniele Francesco Santamaria. A Set-Theoretical Representation For OWL 2 Profiles With Application on Ancient Ceramics Classification. LAP LAMBERT Academic Publishing, ISBN 978-3-659-68797-6, 2015.

- Domenico Cantone and Marianna Nicolosi-Asmundo. On the satisfiability problem for a 4-level quantified syllogistic and some applications to modal logic. Fundamenta Informaticae, 124(4):427-448, 2013.



Disclaimer:

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/.


Software and its documentation made available on this repository:

-could include technical or other mistakes, inaccuracies or typographical errors. The authors may make changes to the software or documentation made available on this repository. -may be out of date, and the authors make no commitment to update such materials. The authors assume no responsibility for errors or ommissions in the software or documentation available from its web site.

In no event shall the authors be liable to you or any third parties for any special, punitive, incidental, indirect or consequential damages of any kind, or any damages whatsoever, including, without limitation, those resulting from loss of use, data or profits, whether or not the BGS has been advised of the possibility of such damages, and on any theory of liability, arising out of or in connection with the use of this software.

The use of the software downloaded through this repository is done at your own discretion and risk and with agreement that you will be solely responsible for any damage to your computer system or loss of data that results from such activities. No advice or information, whether oral or written, obtained by you from the BGS or from the BGS web site shall create any warranty for the software.

Compile instructions:

The software has been implemented in the Microsoft Visual Studio 2017 environment. Please refer to VS17 documentation for compiling instructions. If you decide to compile the program with GCC remove all the #include "stdafx.h" instructions. Always refer to the "main" branch for the last (stable) version of the sofware.

Software downloaded from this repository is provided 'as is' without warranty of any kind, either express or implied, including, but not limited to, the implied warranties of fitness for a purpose, or the warranty of non-infringement. Without limiting the foregoing, the authors make no warranty that:

-the software will meet your requirements; -the software will be uninterrupted, timely, secure or error-free; -the results that may be obtained from the use of the software will be effective, accurate or reliable; -the quality of the software will meet your expectations; -any errors in the software obtained from this repository e will be corrected.
