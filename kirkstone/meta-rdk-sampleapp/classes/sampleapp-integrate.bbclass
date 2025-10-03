# sampleapp-integrate.bbclass
# This class demonstrates where to place reusable integration logic for third-party apps,
# such as default systemd behavior, RDK logging integration, or common packaging tweaks.

inherit systemd

# PUBLIC_INTERFACE
python () {
    """
    This anonymous Python function can be extended to enforce defaults or QA checks.
    """
}
