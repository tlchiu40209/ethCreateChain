# Easy Private Chain Creation Tool for Ethereum 1.9

- Usage : 
    
```sh
createChain [-i/-c/-r] [chain ID] [Options].
-i [interactive] Create a chain interactively.
-c [chain ID] Create a chain, require Options.
-r [chain ID] Remove a chain.
```
 
- [Options] :
```sh
-w [Network ID] Determine the network ID for the chain.
-n [Nounce] Determine the nonce value for the chain.
-d [Difficulty] Determine the difficulty value for the chain.
-g [GasLimit] Determine the GasLimit for the chain.
```

- Note : Please specify path to geth and the store location of the chain in config.ini first !
