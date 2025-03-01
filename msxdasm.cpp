// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// MSXDasm
// Copyright (C) 1999-2025 Eduardo Aguiar
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include "disassembler.hpp"
#include <iomanip>
#include <iostream>
#include <vector>
#include <unistd.h>

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Show usage text
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
static void
usage ()
{
  std::cerr << '\n';
  std::cerr << "Use: msxdasm [OPTIONS] file.ROM\n";
  std::cerr << "e.g: msxdasm kvalley.rom\n";
  std::cerr << '\n';
  std::cerr << "Options are:\n";
  std::cerr << "  -d Read address definition file (eg. msxrom.def). Can be used multiple times\n";
  std::cerr << "     E.g: -d msxrom.def -d kvalley.def\n";
  std::cerr << '\n';
  std::cerr << "  -e Set execution address in hexa (default = cartridge default)\n";
  std::cerr << "     E.g: -e 406c\n";
  std::cerr << '\n';
  std::cerr << "  -o Set output file name. (default = msxdasm.out)\n";
  std::cerr << '\n';
  std::cerr << "  -p Add code entry point, for unreachable code\n";
  std::cerr << "     E.g: -p 401a\n";
  std::cerr << '\n';
  std::cerr << "  -s Set start address in hexa (default = 4000h)\n";
  std::cerr << "     E.g: -s 4000\n";
  std::cerr << '\n';
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Main function
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int
main (int argc, char **argv)
{
  std::cerr << "=====================================================\n";
  std::cerr << "MSXDASM v3.0\n";
  std::cerr << "by Eduardo Aguiar\n";
  std::cerr << "=====================================================\n";

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // Parse command line
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  std::vector <std::string> output_files;
  std::vector <std::string> definition_files;
  std::vector <std::uint16_t> entry_points;

  std::uint16_t start_addr = 0x4000;
  std::uint16_t exec_addr = 0;

  int opt;
  while ((opt = getopt (argc, argv, "hd:e:lo:p:s:")) != EOF)
    {
      switch (opt)
        {
        case 'h':
          usage ();
          exit (EXIT_SUCCESS);
          break;

        case 'd':
          definition_files.push_back (optarg);
          break;

        case 'e':
          exec_addr = std::stoi (optarg, nullptr, 16);
          break;

        case 'o':
          output_files.push_back (optarg);
          break;

        case 'p':
          entry_points.push_back (std::stoi (optarg, nullptr, 16));
          break;

        case 's':
          start_addr = std::stoi (optarg, nullptr, 16);
          break;

        default:
          usage ();
          exit (EXIT_FAILURE);
        }
    }

  if (optind >= argc)
    {
      std::cerr << std::endl;
      std::cerr << "Error: you must enter at least one .rom file to disassembly" << std::endl;
      usage ();
      exit (EXIT_FAILURE);
    }

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // Load .rom file
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  const std::string path = argv[optind];

  msxdasm::disassembler disasm;
  disasm.load_rom (path, start_addr);

  if (exec_addr)
    disasm.set_exec_address (exec_addr);

  for (const auto& path : definition_files)
      disasm.load_def (path);

  for (auto addr : entry_points)
      disasm.add_entry_point (addr);

  disasm.navigate ();

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // Show cartridge data
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  std::cerr << "Cartridge    : " << path << '\n';
  std::cerr << "Start address: " << std::hex << std::setw(4) << std::setfill('0') << disasm.get_start_address () << std::endl;
  std::cerr << "End address  : " << std::hex << std::setw(4) << std::setfill('0') << disasm.get_end_address () << std::endl;
  std::cerr << "Exec address : " << std::hex << std::setw(4) << std::setfill('0') << disasm.get_exec_address () << std::endl;

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // Generate output
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  if (output_files.empty ())
    output_files.push_back ("msxdasm.out");

  for (const auto& path : output_files)
    disasm.generate (path);
}
