import { useEffect, useState } from "react";
import { ScrollView, View, Text, StyleSheet } from "react-native";

import AppLayout from "../components/layout/AppLayout";

import WaterLevelCard from "../components/cards/WaterLevelCard";
import PumpStatusCard from "../components/cards/PumpStatusCard";
import PetDetectionCard from "../components/cards/PetDetectionCard";
import ConsumptionCard from "../components/cards/ConsumptionCard";

import { ThingsBoardService } from "../services/thingsboard";

export default function CompositeDashboardScreen() {
  const [data, setData] = useState<any>(null);

  useEffect(() => {
    const load = async () => {
      const service = new ThingsBoardService();
      const telemetry = await service.getTelemetry();
      setData(telemetry);
    };

    load();
  }, []);

  if (!data) {
    return (
      <AppLayout>
        <Text>Cargando datos...</Text>
      </AppLayout>
    );
  }

  return (
    <AppLayout>
      <ScrollView>

        <View style={styles.section}>
          <Text style={styles.sectionTitle}>Estado General del Sistema</Text>
          <WaterLevelCard value={data.waterLevel} />
          <PumpStatusCard isOn={data.pumpStatus} />
        </View>

        <View style={styles.section}>
          <Text style={styles.sectionTitle}>Actividad de la Mascota</Text>
          <PetDetectionCard detected={data.petDetected} />
        </View>

        <View style={styles.section}>
          <Text style={styles.sectionTitle}>Consumo del Sistema</Text>
          <ConsumptionCard value={data.consumption} />
        </View>

      </ScrollView>
    </AppLayout>
  );
}

const styles = StyleSheet.create({
  section: {
    marginBottom: 20,
    padding: 12,
    backgroundColor: "#ffffff",
    borderRadius: 12,
  },
  sectionTitle: {
    fontSize: 16,
    fontWeight: "600",
    marginBottom: 10,
  },
});