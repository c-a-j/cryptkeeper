#include "cli/cli.hpp"

#include <string>

#include "cli/types.hpp"
#include <CLI/CLI.hpp>

#include "global.hpp"
#include "ui/console/console.hpp"

namespace ck::cli {
using ck::ui::console::console;

namespace {
struct ParsedCmdArgs {
    InitArgs init;
    ConfigArgs config;
    InsertArgs insert;
    ShowArgs show;
    MountArgs mount;
    UmountArgs umount;
    ChrootArgs chroot;
    RemoveArgs remove;
    ReplaceArgs replace;
    EditArgs edit;
};

void configure_root(CLI::App &app, RootArgs &args) {
    app.require_subcommand(1, 1);
    app.description(
        std::string(APP_NAME) +
        " is a command-line tool for managing encrypted secrets."
    );
    app.footer("More coming soon");

    app.add_flag(
        "--version", args.version, "Disable ANSI code color insertion"
    );
    app.add_flag(
        "--no-color", args.no_color, "Disable ANSI code color insertion"
    );
}

CLI::App *add_init(CLI::App &app, ParsedCmdArgs &args) {
    auto *init = app.add_subcommand("init", "initialize a new password store");
    init->add_option("-v,--crypt", args.init.crypt_name, "crypt name")
        ->required();
    init->add_option("-k,--key", args.init.key_fpr, "crypt key")->required();
    init->add_option("-p,--path", args.init.path, "crypt path");
    return init;
}

CLI::App *add_config(CLI::App &app, ParsedCmdArgs &args) {
    auto *config = app.add_subcommand("config", "Print and edit config file");
    config->description("Print and edit config file");
    // config -> add_option("-v, --crypt", args.config.crypt_name, "Set configs
    // for a specific crypt");
    config->add_option("args", args.config.set_args, "Key [value]");
    return config;
}

CLI::App *add_insert(CLI::App &app, ParsedCmdArgs &args) {
    auto *insert = app.add_subcommand("insert", "insert a new secret");
    insert->add_flag(
        "--pwgen", args.insert.pwgen, "insert a randomly generated password"
    );
    insert
        ->add_option(
            "path", args.insert.path,
            "secret path and name (ex cards/mybank/num)"
        )
        ->required();
    return insert;
}

CLI::App *add_show(CLI::App &app, ParsedCmdArgs &args) {
    auto *show = app.add_subcommand("show", "Show a secret");
    show->add_option("path", args.show.path, "Secret path");
    return show;
}

CLI::App *add_mount(CLI::App &app, ParsedCmdArgs &args) {
    args.mount.list = false;
    auto *mount = app.add_subcommand("mount", "Mount a crypt");
    mount->add_option("path", args.mount.path, "Crypt path")->expected(1);
    mount->add_option("alias", args.mount.alias, "Mount alias")->expected(1);
    mount->add_flag("-l, --list", args.mount.list, "List mounts");
    return mount;
}

CLI::App *add_umount(CLI::App &app, ParsedCmdArgs &args) {
    auto *umount = app.add_subcommand("umount", "Unmount a crypt");
    umount->add_option("alias", args.umount.alias, "Mount alias")->required();
    return umount;
}

CLI::App *add_chroot(CLI::App &app, ParsedCmdArgs &args) {
    auto *chroot = app.add_subcommand("chroot", "Change root crypt");
    chroot->add_option("path", args.chroot.path, "Crypt path")->required();
    return chroot;
}

CLI::App *add_remove(CLI::App &app, ParsedCmdArgs &args) {
    auto *remove = app.add_subcommand("remove", "Remove a secret");
    remove->add_option("path", args.remove.path, "Secret path")->required();
    return remove;
}

CLI::App *add_replace(CLI::App &app, ParsedCmdArgs &args) {
    auto *replace = app.add_subcommand("replace", "Replace a secret");
    replace->add_option("path", args.replace.path, "Secret path")->required();
    return replace;
}

CLI::App *add_edit(CLI::App &app, ParsedCmdArgs &args) {
    auto *edit = app.add_subcommand("edit", "Edit a secret");
    edit->add_option("path", args.edit.path, "Secret path")->required();
    return edit;
}
} // namespace

CliArgs parse_cli(CLI::App &app, int argc, char **argv) {
    ParsedCmdArgs cargs;
    RootArgs rargs;

    configure_root(app, rargs);
    auto *init = add_init(app, cargs);
    auto *config = add_config(app, cargs);
    auto *insert = add_insert(app, cargs);
    auto *show = add_show(app, cargs);
    auto *mount = add_mount(app, cargs);
    auto *umount = add_umount(app, cargs);
    auto *chroot = add_chroot(app, cargs);
    auto *remove = add_remove(app, cargs);
    auto *replace = add_replace(app, cargs);
    auto *edit = add_edit(app, cargs);

    CliArgs args = CliArgs{
        .root = RootArgs{},
        .cmd = std::monostate{},
    };

    if (argc == 1) {
        std::cout << app.help() << '\n';
        return args;
    }

    app.parse(argc, argv);

    args.root = rargs;

    if (init->parsed()) {
        args.cmd = cargs.init;
        return args;
    }
    if (config->parsed()) {
        args.cmd = cargs.config;
        return args;
    }
    if (insert->parsed()) {
        args.cmd = cargs.insert;
        return args;
    }
    if (show->parsed()) {
        args.cmd = cargs.show;
        return args;
    }
    if (mount->parsed()) {
        args.cmd = cargs.mount;
        return args;
    }
    if (umount->parsed()) {
        args.cmd = cargs.umount;
        return args;
    }
    if (chroot->parsed()) {
        args.cmd = cargs.chroot;
        return args;
    }
    if (remove->parsed()) {
        args.cmd = cargs.remove;
        return args;
    }
    if (replace->parsed()) {
        args.cmd = cargs.replace;
        return args;
    }
    if (edit->parsed()) {
        args.cmd = cargs.edit;
        return args;
    }

    return args;
}
} // namespace ck::cli
