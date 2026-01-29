import { useState } from 'react';
import { LoginPage } from './components/LoginPage';
import { MainMenu } from './components/MainMenu';
import { ChargingStationSelect } from './components/ChargingStationSelect';
import { VehicleSelect } from './components/VehicleSelect';
import { ConsumptionTracking } from './components/ConsumptionTracking';

type Page = 'login' | 'menu' | 'charging-station' | 'vehicle' | 'consumption';

export default function App() {
  const [currentPage, setCurrentPage] = useState<Page>('login');
  const [isAuthenticated, setIsAuthenticated] = useState(false);
  const [user, setUser] = useState<{ name: string; email: string } | null>(null);

  const handleLogin = (email: string, password: string) => {
    // Mock login
    setUser({ name: 'Utilisateur', email });
    setIsAuthenticated(true);
    setCurrentPage('menu');
  };

  const handleSignup = (name: string, email: string, password: string) => {
    // Mock signup
    setUser({ name, email });
    setIsAuthenticated(true);
    setCurrentPage('menu');
  };

  const handleLogout = () => {
    setUser(null);
    setIsAuthenticated(false);
    setCurrentPage('login');
  };

  const navigateTo = (page: Page) => {
    setCurrentPage(page);
  };

  if (!isAuthenticated) {
    return <LoginPage onLogin={handleLogin} onSignup={handleSignup} />;
  }

  return (
    <div className="min-h-screen bg-gray-50">
      {currentPage === 'menu' && (
        <MainMenu user={user} onNavigate={navigateTo} onLogout={handleLogout} />
      )}
      {currentPage === 'charging-station' && (
        <ChargingStationSelect onBack={() => navigateTo('menu')} />
      )}
      {currentPage === 'vehicle' && (
        <VehicleSelect onBack={() => navigateTo('menu')} />
      )}
      {currentPage === 'consumption' && (
        <ConsumptionTracking onBack={() => navigateTo('menu')} />
      )}
    </div>
  );
}
