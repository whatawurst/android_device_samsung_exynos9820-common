/*
 * Copyright (C) 2020 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.lineageos.wirelesspowershare;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.drawable.Icon;
import android.os.BatteryManager;
import android.os.IBinder;
import android.os.PowerManager;
import android.os.SystemProperties;
import android.service.quicksettings.Tile;
import android.service.quicksettings.TileService;

public class WirelessPowerShare extends TileService {

    public static final String POWERSHARE_PROP = "sys.powershare";
    public static final int MIN_BATTERY = 30;

    private final PowerSaveReceiver powerSave = new PowerSaveReceiver(this);
    private boolean available = true;

    @Override
    public void onStartListening() {
        super.onStartListening();
        getQsTile().setIcon(Icon.createWithResource(this, R.drawable.ic_powershare));

        if (available) {
            if (SystemProperties.get(WirelessPowerShare.POWERSHARE_PROP).equals("0")) {
                getQsTile().setState(Tile.STATE_INACTIVE);
            } else {
                getQsTile().setState(Tile.STATE_ACTIVE);
            }
        } else {
            getQsTile().setState(Tile.STATE_UNAVAILABLE);
        }

        getQsTile().updateTile();
    }

    @Override
    public void onClick() {
        if (available) {
            super.onClick();

            if (SystemProperties.get(WirelessPowerShare.POWERSHARE_PROP).equals("0")) {
                SystemProperties.set(WirelessPowerShare.POWERSHARE_PROP, "1");
                getQsTile().setState(Tile.STATE_ACTIVE);
            } else {
                SystemProperties.set(WirelessPowerShare.POWERSHARE_PROP, "0");
                getQsTile().setState(Tile.STATE_INACTIVE);
            }

            getQsTile().updateTile();
        }
    }

    public int getBatteryPercent() {
        BatteryManager bm = (BatteryManager) this.getSystemService(BATTERY_SERVICE);
        return bm.getIntProperty(BatteryManager.BATTERY_PROPERTY_CAPACITY);
    }

    public boolean isPowerSave() {
        final PowerManager pm = (PowerManager) this.getSystemService(Context.POWER_SERVICE);
        return pm.isPowerSaveMode();
    }

    @Override
    public IBinder onBind(Intent intent) {
        IntentFilter filterPowerSave = new IntentFilter();

        filterPowerSave.addAction(PowerManager.ACTION_POWER_SAVE_MODE_CHANGED);
        filterPowerSave.addAction(Intent.ACTION_BATTERY_CHANGED);

        registerReceiver(powerSave, filterPowerSave);

        this.requestListeningState();

        this.setAvailable(this.getBatteryPercent() > WirelessPowerShare.MIN_BATTERY && !this.isPowerSave());

        return super.onBind(intent);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        unregisterReceiver(powerSave);
    }

    public void setAvailable(boolean available) {
        if (this.available != available) {
            this.available = available;

            if (!available) {
                SystemProperties.set(WirelessPowerShare.POWERSHARE_PROP, "0");
            }

            this.requestListeningState();
        }
    }

    public void requestListeningState() {
        TileService.requestListeningState(this.getBaseContext(), new ComponentName("org.lineageos.wirelesspowershare", "org.lineageos.wirelesspowershare.WirelessPowerShare"));
    }
}
