#!/bin/bash
set -e

echo "=== Cay uninstall script ==="
sudo rm -rf /opt/cay
sudo rm -f /etc/profile.d/cay.sh
sudo sed -i '\|^export PATH="/opt/cay:$PATH"$|d' /etc/bash.bashrc
echo "✅ Cay uninstall completed."
