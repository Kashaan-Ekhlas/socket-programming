# The NME Protocol 1.0
New Message Era Protocol is an application layer protocol over TCP, partitioned into two sections: Header and Body.

## Request
### Header
```
{
    uint8 packetType;
    uint32 packetSize;
    uint8 sessionId[15]; 
}
// no padding
```
### Body

when packetType = 0, it indicates normal msg, hence body is a message:
```
{
    buffer[size]; // size defined by packetSize
}
```

when packetType = 1 or 2 or 3, it indicates auth - sign in or sign up or username reset respectively, hence body is username/password, here sessionId is just a zero array:
```
{
    char user[size];
    char pass[also_size];
} // server defined sizes.
```