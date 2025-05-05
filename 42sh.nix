{
  lib,
  stdenv,
  withDevBinaries ? false,
}:
stdenv.mkDerivation {
  name = "42sh";
  version = "dev";

  src = ./.;
  enableParallelBuilding = true;

  makeFlags = [ "PREFIX=${placeholder "out"}"]
    ++ (lib.optional withDevBinaries ["debug" "check"]);

  postInstall = lib.optional withDevBinaries ''
    install -Dm755 -t $out/bin debug
  '';

  meta = {
    description = "Epitech 42sh";
    license = [ ];
    mainProgram = "42sh";
    platforms = lib.platforms.unix;
    maintainers = with lib.maintainers; [ savalet ];
  };
}
