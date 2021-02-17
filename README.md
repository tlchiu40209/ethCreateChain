# Easy Private Chain Creation Tool for Ethereum 1.9

- Usage : 
    
```sh
createChain [-i/-c/-r] [chain ID] [Options].
-i [Options...] Create a chain interactively.
-c [chain ID] [Options...] Create a chain, require Options.
-r [chain ID] [Options...] Remove a chain.
```
 
- [Options for -i] :
```sh
-p Create the chain with PoA Consensus Algorithm.
```

- [Options for -c :
```sh
-a [Password:Amount] Specifying the preconfigured accounts.
-b [Number] [Password] [Amount] Accounts batch creation.
-d [Difficulty] Determine the difficulty value for the chain.
-g [GasLimit] Determine the GasLimit for the chain.
-n [Nounce] Determine the nonce value for the chain.
-p Create PoA chain.
-w [Network ID] Determine the network ID for the chain.
```

- [Options for -r] :
```sh
-p The chain to remove is a PoA chain.
```

- Note : Please specify path to geth and the store location of the chain in config.ini first !
