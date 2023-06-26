# nm class log
- 規格書寫到NmCluster為abstract，目前內未有virtual func
- TimeValue暫定為uint32_t
- ipv4AddressStirng and ipv6AddressString implemented by string type in C++ (currently)
- PncMapping中的PhysicalChannel和EthernetPhysicalChannel是否能夠視作同一型態?
- shortLabel implemented by string
- nmNode collection implemented through vector in nmCluster 
- nmCluster collection implemented through vector
- pnc中兩個optional的變數和servicemap尚未實作
- 目前兩個global變數<NmConfig>以及<NmInstantiation>之命名在規格書中沒有看到，暫時依照命名規則來制定名稱
