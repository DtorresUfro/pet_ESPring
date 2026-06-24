import { useEffect, useState } from "react";
import { ScrollView } from "react-native";
import AppLayout from "../components/layout/AppLayout";

import WaterLevelCard from "../components/cards/WaterLevelCard";
import PumpStatusCard from "../components/cards/PumpStatusCard";
import PetDetectionCard from "../components/cards/PetDetectionCard";
import ConsumptionCard from "../components/cards/ConsumptionCard";

import { ThingsBoardService } from "../services/thingsboard";

export default function BasicDashboardScreen() {

  const [data, setData] = useState<any>(null);

  useEffect(() => {
    const load = async () => {
      const service = new ThingsBoardService();
      const telemetry = await service.getTelemetry();
      setData(telemetry);
    };

    load();
  }, []);

  if (!data) return null;

  return (
    <AppLayout>
      <ScrollView>
        <WaterLevelCard value={data.waterLevel} />
        <PumpStatusCard isOn={data.pumpStatus} />
        <PetDetectionCard detected={data.petDetected} />
        <ConsumptionCard value={data.consumption} />
      </ScrollView>
    </AppLayout>
  );
}