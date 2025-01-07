# Relocatable Demo3D Cutscenes

Original Creators: [HelloOO7](https://github.com/HelloOO7)
(Minor revisions by [Brom](https://github.com/brombrombromley))

With this patch, the game's 3D cutscenes are moved from Overlay 293 into the file system for Pokémon White Version 2.

## Build Instructions

These instructions are made with Windows users in mind.

### Build Dependencies:
- GNU Make
- [knarc](https://github.com/kr3nshaw/knarc/releases/tag/1.0.0) 
- [arm-none-eabi](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)

By default, running `make` will create:
1. An ELF file to be used with the included `demo3d_esdb.yml` ESDB to install using CTRMap
2. A NARC of the `rlseqbin` files included in `resource/demo3d/bin`.

To build just the code itself, after installing Make, run `make demo3d`, and it will just create the ELF to be used with the included ESDB for CTRMap.

To build or update a NARC with the Demo3D sequence data, add a new environment variable (which can be done by typing in "environment variables" in the Windows search bar and selecting `Edit the system environment variables`) and making a new environment variable called `VFS_ROOT`. Set its value to the `vfs` folder of the CTRMap project you would like to use this with.

> (NOTE: You may need to restart the program you are using the terminal you are using to run `make` for it to reflect the changed environment variable)

After setting `VFS_ROOT`, you can use `make demo3d_narc` to build a new NARC from the files in `resource/demo3d/bin` and it will automatically place it within your project's `vfs/data` folder.

## Using in a CTRMap project

### Usage Dependencies:

- [CTRMap-CE](https://github.com/ds-pokemon-hacking/CTRMap-CE/releases)
- [CTRMapV Plugin](https://github.com/ds-pokemon-hacking/CTRMapV/releases)
- [PMC](https://github.com/ds-pokemon-hacking/PMC/releases)
- [NitroKernel](https://github.com/HelloOO7/NitroKernel/releases)

### Adding the modified Overlay 293

Using `overlay_0293.bin` placed in the root of this repository, place it in your project's `vfs/data/overlay` folder. You may need to create this folder if it is not there already.

### Setting up the code injection environment

If you haven't already, head to the `Extras` panel in CTRMap then click the `Install/Update PMC` button to install the [latest PMC version](https://github.com/ds-pokemon-hacking/PMC/releases). By doing this, this should also create a folder called `patches` within your project's `vfs/data` folder. After that, place `NitroKernel.dll` from the [NitroKernel releases page](https://github.com/HelloOO7/NitroKernel/releases) in your project's new `vfs/data/patches` folder. For more details on this step, consult the official [code injection guide](https://ds-pokemon-hacking.github.io/docs/generation-v/guides/bw_b2w2-code_injection/#linking) or visit [DS Modding Community](https://discord.gg/YBtdN3aXfv) for more support.

### Turning `demo3d_reloc.elf` into a DLL patch

After the code injection environment is set up, in the `Extras` panel, check the box next to `Install to /patches` if it isn't checked already, then click `Convert ELF to DLL`. First, it will ask you to get the external symbol database (ESDB) YAML that will be used to hook the code we want to inject. For this, we will use `demo3d_esdb.yml` which has the hooks we would like our code to link to. After that, it will ask for which ELF we would like to convert to a DLL patch. 

> NOTE: If you accidentally select the wrong file to use as the ESDB, holding shift while clicking `Convert ELF to DLL` can let you select another YAML file to use.

After building the ELF by running `make` or `make demo3d` earlier, the ELF should be in `out/demo3d` of this repository. Select `demo3d_reloc.elf` as the ELF to use, then when it asks for you to save the DLL, you can save it in the same folder. Because we selected `Install to /patches` earlier, the DLL patch will also be added to our `vfs/data/patches` folder next to `NitroKernel.dll`.

> NOTE: To make sure all the hooks got properly resolved, you can open CTRMap through the command line using `java -jar <CTRMAP-CE JAR NAME>` to see if the functions hooked successfully.

As the last step, make your project's `vfs/data` folder has a NARC called `demo3d_seq_data.narc`. If it doesn't, make sure to follow the steps for building this NARC in the `Build Instructions` section of this README. After this, export a new ROM.

> NOTE: By default, this patch will expect the Demo3D sequence data NARC to be called `demo3d_seq_data.narc`, but this can be changed within the Makefile and `Demo3D_LoadSeqFile()` (on line 39 of `demo3d.cpp`)

## Verifying the patch is reading from the NARC

To quickly verify that the patch is working correctly, replace `seq_005.rlseqbin` with `seq_008.rlseqbin` then run `make demo3d_narc` and export a new ROM. If everything worked correctly, the starting cutscene should be replaced with another, (cooler) one!

## Notes

Right now, while CTRMap technically is able to convert the YAML files in `resource/demo3d/yaml`, [this code](https://github.com/ds-pokemon-hacking/CTRMapV/blob/master/src/ctrmap/formats/pokemon/gen5/sequence/SeqReader.java) is not exposed through any of the editors. This issue should hopefully be resolved in the future, but for now it will be necessary to build a modified CTRMapV in order to convert the Demo3D sequence YAMLs to the `rlseqbin` format.
